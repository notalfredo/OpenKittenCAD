//---- DEFINITIONS  ----------------------------------------------
%{
#define YYDEBUG 1
#include <stdarg.h>
#include <stdio.h>

#include "parser.tab.h"
#include "lexer.yy.h"

// Defined below in the User Code section.
extern void yyerror( YYLTYPE *, void *, void *, const char * );
%}

    /*
    We use reentrant so we dont have to use global data items 
    Affects the signature of yyerror.
    We have to suply more information to lex-param and parse-param 
    */
%define api.pure full


    /*
    Pointer to an instance of state is called yyscan_t
    */
%code requires {
  #include "node.hxx"
  #include <iostream>
  typedef void *yyscan_t;
}

%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { void **result }

    /*This header contains token name, and unions*/
%defines "parser.tab.h"

    /*
    Enable the use of yylloc.  In the rules below, we get location
    information via the '@' marker.
    */
%locations

%union {
  Node*           node;

  NodeIf*         ifNode;
  NodeElif*       elifNode;
  NodeElse*       elseNode;

  NodeFor*        forNode; 
  NodeWhile*      whileNode; 
  NodeRepeat*     repeatNode;
  NodeUntil*      untilNode;


  NodeStatement*  stmtNode;
  NodeStmtList*   stmtListNodes;

  NodeFunction*   functionNode;


  NodeDecl*       declNode;
  NodeReAssign*   reAssignNode;

  NodeIdentifier* idNode;
  NodeType*       typeNode;
  NodeDeclList*   declList ;

  NodeBlock*      blockNode;

  NodeNumber*     numberNode;
  NodeArray*      arrayNode;
  
  NodeType*       idType;

  NodeExpression* exprNode;

  NodeExprStmt*   exprStmtNode;

  NodeReturnStmt* returnStmtNode;

  char*           charPointer;
}


    /* Terminals */
%token tok_ILLEGAL
%token tok_BREAK tok_CONTINUE tok_REPEAT tok_UNTIL
%token tok_IF tok_ELIF tok_ELSE
%token tok_FOR tok_WHILE
%token tok_AND tok_OR
%token tok_FN tok_RETURN
%token tok_LET tok_CONST
%token tok_EQ tok_NE tok_GE tok_LE 
%token tok_PIPE
%token tok_ASSIGN
%token tok_ARROW


%token <idNode>         tok_ID
%token <numberNode>     tok_NUM
%token <idType>         tok_TYPE




    /* Non-terminals */
%type <ifNode>                       ifStmt
%type <elifNode>                     elifStmt
%type <elseNode>                     elseStmt

%type <forNode>                      forStmt
%type <whileNode>                    whileStmt
%type <repeatNode>                   repeatStmt
%type <untilNode>                    untilStmt

%type <declNode>                     paramDecl declStmt
%type <reAssignNode>                 reAssignStmt

%type <declList>                     paramDeclList
%type <stmtListNodes>                stmtList 

%type <stmtNode>                     stmt

%type <functionNode>                 functionStmt

%type <idNode>                       identifier
%type <blockNode>                    block

%type<exprNode>                      expr argList initOpt return arrayList

%type<exprStmtNode>                  exprStmt

%type<returnStmtNode>                returnStmt

%left '+' '-'
%left '*' '/'
%nonassoc tok_ASSIGN
%left tok_PIPE


%% //---- RULES --------------------------------------------------


start:
    stmtList ';'   { 
        *result = $1; 
    }

block:
    '{' stmtList ';' '}'    { $$ = newNodeBlock($2);   }
  | '{'              '}'    { $$ = newNodeBlock(NULL); }
  ;

stmt:
    block           { $$ = $1; }
  | functionStmt    { $$ = $1; }
  | declStmt        { $$ = $1; }
  | reAssignStmt    { $$ = $1; }
  | exprStmt        { $$ = $1; }
  | returnStmt      { $$ = $1; }
  ;

stmtList:
  stmtList ';' stmt {
        appendToStmtList($1, $3);
        $$ = $1; 
  }
  | stmt                {
        NodeStmtList* myNewList = newStmtList($1);
        $$ = myNewList;
  }
  ;

functionStmt:
    tok_FN tok_ID '(' paramDeclList ')' tok_ARROW tok_TYPE block  { 
        $$ = newFunctionNode($2, $4, $7, $8); 
    }
    |
    tok_FN tok_ID '(' paramDeclList ')' block  { 
        $$ = newFunctionNode($2, $4, newNodeType(_void), $6); 
    }
  ;

paramDeclList:
    %empty                             {
        NodeDeclList* temp = newDeclList(NULL); 
        $$ = temp;
    } 
  | paramDeclList ',' paramDecl  {
        addDeclToList($1, $3);
        $$ = $1;

  }
  | paramDecl                       { 
        NodeDeclList* temp = newDeclList($1); 
        $$ = temp;
  }
  ;

returnStmt:
    tok_RETURN expr     { $$ = newReturnNode($2); }
  ;

paramDecl:
    tok_ID ':' tok_TYPE    { $$ = newDeclNode($1, $3, NULL, MUT); }
  ;


reAssignStmt:
    tok_ID tok_ASSIGN expr { $$ = newReAssignNode($1, $3); }
  ;


declStmt:
    tok_LET tok_ID ':' tok_TYPE initOpt {
        if(!$5) { $$ = newDeclNode($2, $4, NULL, MUT); }
        $$ = newDeclNode($2, $4, $5, MUT);
    }
  | tok_CONST tok_ID ':' tok_TYPE initOpt {
        if(!$5) { $$ = newDeclNode($2, $4, NULL, NON_MUT); }
        $$ = newDeclNode($2, $4, $5, NON_MUT);
    }
  ;

initOpt: 
  %empty                { $$ = NULL; }
  | tok_ASSIGN expr     { $$ = $2;   }
  ;

exprStmt:
    expr {
        NodeExprStmt* temp = newExprStmtNode($1);
        $$ = temp;
    }
  ;

expr:
    '-' expr                       { $$ = newUnaryOperatorNode($2, OP_NEGATE); }
  |
    expr '+' expr                  { $$ = newBinaryOperatorNode($1, $3, OP_PLUS); }
  | expr '-' expr                  { $$ = newBinaryOperatorNode($1, $3, OP_SUB); }
  | expr '*' expr                  { $$ = newBinaryOperatorNode($1, $3, OP_MUL); }
  | expr '/' expr                  { $$ = newBinaryOperatorNode($1, $3, OP_DIV); }
  | expr tok_PIPE expr             { $$ = newBinaryOperatorNode($1, $3, OP_PIPE); }
  | expr tok_ASSIGN expr           { $$ = newBinaryOperatorNode($1, $3, OP_ASSIGN); }
  | tok_NUM                        { $$ = $1; }
  | '[' arrayList ']'              { $$ = newArrayNode($2); } 
  | tok_ID                         { $$ = $1; }
  | tok_ID '(' argList ')'         { $$ = newFunctionCallNode($1, $3); }
  ;


arrayList:
    arrayList ',' expr  {
    appendExprLinkedList(&$1, $3);
    $$ = $1;
  }
  | expr  { $$ = $1; }
  ;

argList:
    %empty            { $$ = NULL; }
  | argList ',' '%'   { 
    appendExprLinkedList(&$1, newPlaceHolderNode());
    $$ = $1;
  }
  | argList ',' expr  { 
    appendExprLinkedList(&$1, $3);
    $$ = $1;
  }
  | expr              { $$ = $1; }
  | '%'               { $$ = newPlaceHolderNode(); }
  ;


%% //---- USER CODE ----------------------------------------------

// Prints an error message.  Automatically called by the
//  generated parser when necessary.
void yyerror(  YYLTYPE *llocp, void *_scanner, void *_result, const char *msg )
{
  (void) _scanner;      // Not used at present
  (void) _result;       // Not used at present
  (void) llocp;
  (void) msg;
}

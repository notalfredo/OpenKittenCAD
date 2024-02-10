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

// Make the parser reentrant so we don't have those pesky global
//  data items.  This affects the signature to the yyerror
//  routine.  We also have to supply additional info to the lexer
//  and parser when we do this.  See the following %lex-param,
//  %parse-param definitions.
%define api.pure full


// With reentrancy, we have to pass around all of the scanner
//  state.  The type of a pointer to an instance of that state is
//  called yyscan_t.
%code requires {
  #include "node.hxx"
  #include <iostream>
  typedef void *yyscan_t;
}

// Add the following parameters to the lexer and parser calls.
//  This is necessary when we're getting rid of global references.
%lex-param   { yyscan_t scanner }
%parse-param { yyscan_t scanner }
%parse-param { void **result }

// Generate a header file with the token names, union of possible
//  attributes, etc.  (This header will be included by the lexer.)
%defines "parser.tab.h"

// Enable the use of yylloc.  In the rules below, we get location
//  information via the '@' marker.
%locations

// The union of all possible attributes that can be returned by
//  any category of token.
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
  NodeIdentifier* idNode;
  NodeType*       typeNode;
  NodeDeclList*   declList;


  std::vector<NodeIdentifier*>* idNodeList;

  NodeBlock*      blockNode;

  NodeDouble*     doubleNode;
  
  NodeType*       idType;

  char*           charPointer;
}


    /* Terminals */
%token tok_ILLEGAL
%token tok_BREAK tok_CONTINUE tok_REPEAT tok_UNTIL
%token tok_IF tok_ELIF tok_ELSE
%token tok_FOR tok_WHILE
%token tok_AND tok_OR
%token tok_FN tok_RETURN
%token tok_LET
%token tok_EQ tok_NE tok_GE tok_LE 
%token tok_PIPE
%token tok_ASSIGN
%token tok_ARROW


%token <idNode>         tok_ID

%token <NodeDouble>     tok_NUM
%token <idType>         tok_TYPE

    /* Non-terminals */
%type <ifNode>                       ifStmt
%type <elifNode>                     elifStmt
%type <elseNode>                     elseStmt

%type <forNode>                      forStmt
%type <whileNode>                    whileStmt
%type <repeatNode>                   repeatStmt
%type <untilNode>                    untilStmt

%type <declNode>                     functionDecl variableDecl
%type <declList>                     variableDeclList
%type <stmtListNodes>                stmtList 

%type <stmtNode>                     stmt

    //NODE NEEDED ANY MORE ???
%type <idNodeList>                   identifierList;


%type <functionNode>                 functionStmt

%type <idNode>                       identifier

%type <blockNode>                    block

%% //---- RULES --------------------------------------------------


start:
    stmtList    { 
        *result = $1; 
    }

block:
    '{' stmtList ';' '}'    { $$ = new NodeBlock($2);   }
  | '{'              '}'    { $$ = new NodeBlock(NULL); }
  ;

stmt:
    block           { $$ = $1; }
  | functionStmt    { $$ = $1; }
  ;

stmtList:
  stmtList ';' stmt ';'  {
        appendToStmtList($1, $3);
        $$ = $1; 
  }
  | stmt                {
        NodeStmtList* myNewList = new NodeStmtList($1);
        $$ = myNewList;
  }
  ;

functionStmt:
    tok_FN tok_ID '(' variableDeclList ')' tok_ARROW tok_TYPE block  { 
        $$ = new NodeFunction($2, $4, $7, $8); 
    }
    |
    tok_FN tok_ID '(' variableDeclList ')' block  { 
        $$ = new NodeFunction($2, $4, new NodeType(_void), $6); 
    }
  ;

variableDeclList:
    %empty                             {
        NodeDeclList* temp = new NodeDeclList(NULL); 
        $$ = temp;
    } 
  | variableDeclList ',' variableDecl  {
        addDeclToList($1, $3);
        $$ = $1;

  }
  | variableDecl                       { 
        NodeDeclList* temp = new NodeDeclList($1); 
        $$ = temp;
  }
  ;

variableDecl:
    tok_ID ':' tok_TYPE    { $$ = new NodeDecl($1, $3); }
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

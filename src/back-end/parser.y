//---- DEFINITIONS  ----------------------------------------------
%{
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

  NodeIf*         if_node;
  NodeElif*       elif_node;
  NodeElse*       else_node;

  NodeFor*        for_node; 
  NodeWhile*      while_node; 
  NodeRepeat*     repeat_node;
  NodeUntil*      until_node;

  NodeDecl*       decl_node;

  NodeIdentifier* id_node;
  NodeDouble*     double_node;
}


    /* Terminals */
%token tok_ILLEGAL
%token tok_BREAK tok_CONTINUE
%token tok_IF tok_ELIF tok_ELSE
%token tok_FOR
%token tok_AND tok_OR
%token tok_FN
%token tok_LET
%token tok_EQ tok_NE tok_GE tok_LE 
%token tok_PIPE
%token tok_ASSIGN

%token <NodeIdentifier> tok_ID
%token <NodeDouble> tok_NUM

    /* Non-terminals */
%type <NodeIf>     ifStmt
%type <NodeElif>   elifStmt
%type <NodeElse>   elseStmt

%type <NodeFor>    forStmt
%type <NodeWhile>  whileStmt
%type <NodeRepeat> repeatStmt
%type <NodeUntil>  untilStmt

%type <NodeDecl>   declStmt



%% //---- RULES --------------------------------------------------

start
  : block                    
  ;

block : ;


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

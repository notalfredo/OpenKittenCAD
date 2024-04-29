/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 25 "parser.y"

  #include "node.hxx"
  #include <iostream>
  typedef void *yyscan_t;

#line 55 "parser.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    tok_ILLEGAL = 258,             /* tok_ILLEGAL  */
    tok_BREAK = 259,               /* tok_BREAK  */
    tok_CONTINUE = 260,            /* tok_CONTINUE  */
    tok_REPEAT = 261,              /* tok_REPEAT  */
    tok_UNTIL = 262,               /* tok_UNTIL  */
    tok_IF = 263,                  /* tok_IF  */
    tok_ELIF = 264,                /* tok_ELIF  */
    tok_ELSE = 265,                /* tok_ELSE  */
    tok_FOR = 266,                 /* tok_FOR  */
    tok_WHILE = 267,               /* tok_WHILE  */
    tok_AND = 268,                 /* tok_AND  */
    tok_OR = 269,                  /* tok_OR  */
    tok_FN = 270,                  /* tok_FN  */
    tok_RETURN = 271,              /* tok_RETURN  */
    tok_LET = 272,                 /* tok_LET  */
    tok_CONST = 273,               /* tok_CONST  */
    tok_EQ = 274,                  /* tok_EQ  */
    tok_NE = 275,                  /* tok_NE  */
    tok_GE = 276,                  /* tok_GE  */
    tok_LE = 277,                  /* tok_LE  */
    tok_PIPE = 278,                /* tok_PIPE  */
    tok_ASSIGN = 279,              /* tok_ASSIGN  */
    tok_ARROW = 280,               /* tok_ARROW  */
    tok_MOD = 281,                 /* tok_MOD  */
    tok_ID = 282,                  /* tok_ID  */
    tok_NUM = 283,                 /* tok_NUM  */
    tok_TYPE = 284                 /* tok_TYPE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 44 "parser.y"

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
  NodeDeclList*   declList ;

  NodeBlock*      blockNode;

  NodeNumber*     numberNode;
  NodeArray*      arrayNode;
  
  NodeType*       idType;

  NodeExpression* exprNode;

  NodeExprStmt*   exprStmtNode;

  NodeReturnStmt* returnStmtNode;

  char*           charPointer;

#line 142 "parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (yyscan_t scanner, void **result);


#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

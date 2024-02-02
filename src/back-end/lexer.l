
    /* To avoid the use of global variables */
%option reentrant

/* For a bridge with bison */
%option bison-bridge
%option bison-locations

    /* Creating header file */
%option header-file="lexer.yy.h"

    /* We dont have an interactive session */
%option never-interactive

    /* Do not generate the default rule we handle it.*/
%option nodefault

    /* We dont want yyinput() and yyunput() */
%option noinput nounput

    /* Don't include <unistd.h> we dont need it. */
%option nounistd

    /* Do not call yywrap() on EOF. */
%option noyywrap

    /* Error messages */
%option perf-report perf-report
%option verbose verbose
%option warn

    /* Maintain current line number in yylineno. */
%option yylineno


%{

#include <stdlib.h>

#include <string.h>

#include <iostream>

#include "parser.tab.h"



int numIllegalCharacters;
std::string flexString;

const Keyword keywords[] = {
  { "and",      tok_AND      },
  { "not",      tok_NOT      },
  { "or",       tok_OR       },
  { "break",    tok_BREAK    },
  { "continue", tok_CONTINUE },
  { "if",       tok_IF       },
  { "elif",     tok_ELIF     },
  { "else",     tok_ELSE     },
  { "repeat",   tok_REPEAT   },
  { "until",    tok_UNTIL    },
  { "while",    tok_WHILE    },
  { "for",      tok_FOR      },
  { "let",      tok_LET      },
  { "fn",       tok_FN       },
};

const int NUM_KEYWORDS = (sizeof(keywords) / sizeof(Keyword));

int isKeyWord(char * keyword){
    for(int index = 0; index < NUM_KEYWORDS; index++){
        if(!strcmp(keywords[index].keyWord, keyword)) {
            return keywords[index].token;
        }
    }
    return 0;
}

%}

DIGIT       [0-9]
EXPONENT    [eE][-+]?{DIGIT}+


ALPHA       [_a-zA-Z]
ALPHA_NUM   {ALPHA}|{DIGIT}



    /*===================RULES===================*/
%% 

[-+*/%^?{}();,>=]   {
  flexString = yytext;
  // Single character token.  Just return the char itself.
  return yytext[0];
}

"==" {
    flexString = yytext;
    return tok_EQ; 
}

"!=" {
    flexString = yytext;
    return tok_NE;
}

">=" {
    flexString = yytext;
    return tok_GE;
}

"<=" {
    flexString = yytext;
    return tok_LE;
}

"|>" {
    flexString = yytext;
    return tok_PIPE;
}

":=" {
    flexString = yytext;
    return tok_ASSIGN;
}

{DIGIT}+ |
({DIGIT}*(({DIGIT}\.)|(\.{DIGIT})){DIGIT}*){EXPONENT}? {
    flexString = yytext;
    return tok_NUM; 
}

[ \n\t\r]+ {
    /*Space do nothing for white space*/
}

    /* ID or Keyword */
{ALPHA}{ALPHA_NUM}*  {
    flexString = yytext;

    int result = isKeyWord(yytext);
    if(result) {
        return result;
    }

    return tok_ID;
}

    /* Gargabe */
.       {
    std::cout << "tok_ILLEGAL |" << yytext << "| at ";
    std::cout << "line = " << yylineno << " column = " << yycolumn << std::endl; 
    return tok_ILLEGAL;
}


%%

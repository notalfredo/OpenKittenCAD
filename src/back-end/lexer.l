  // Dalio, Brian A.
  // dalioba
  // 2023-04-16

  //---- DEFINITIONS  --------------------------------------------

/* Ensure the lexer is reentrant -- avoid the original mess of
    global variables for everything. */
%option reentrant

/* Create the lexer exports header file with this name. */
%option header-file="lexer.yy.h"

/* Our input is not from an interactive source. */
%option never-interactive
/* Don't generate the default rule. */
%option nodefault
/* We don't directly read the input. */
%option noinput
/* Don't include <unistd.h>. */
%option nounistd
/* We don't 'unput' a character back. */
%option nounput
/* Don't call yywrap() on EOF. */
%option noyywrap

/* These two options are doubled to get more detailed reports. */
/* Write performance report to stderr. */
%option perf-report perf-report
/* Write statistics summary to stderr. */
%option verbose verbose

/* Generate warning messages for goofs. */
%option warn

/* Maintain current line number in yylineno. */
%option yylineno




%{
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "keywords.hxx"


int numIllegalCharacters;

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
  // Single character token.  Just return the char itself.
  return yytext[0];
}

"==" {
    return tok_EQ; 
}

"!=" {
    return tok_NE;
}

">=" {
    return tok_GE;
}

"<=" {
    return tok_LE;
}

"|>" {
    return tok_PIPE;
}

":=" {
    return tok_ASSIGN;
}

{DIGIT}+ |
({DIGIT}*(({DIGIT}\.)|(\.{DIGIT})){DIGIT}*){EXPONENT}? {
    return tok_NUM; 
}

[ \n\t\r]+ {
    /*Space do nothing for white space*/
}

    /* ID or Keyword */
{ALPHA}{ALPHA_NUM}*  {
    //int result = isKeyWord(yytext);
    //if(result) {
    //    return result;
    //}
    isKeyWord(yytext);

    return tok_ID;
}

    /* Gargabe */
.       {
    std::cout << "tok_ILLEGAL |" << yytext << "| at ";
    std::cout << "line = " << yylineno << " column = " << yycolumn << std::endl; 
    return tok_ILLEGAL;
}


%%

    /*ttFlexLexer for running test*/

    /* We dont have an interactive session. */ 
%option never-interactive


    /* We will handle the nodefault rule. */

    /* We dont want yyinput() and yyunput(). */
%option noinput nounput


    /* Don't include <unistd.h> we dont need it. */
%option nounistd


    /* 
        Not needed for now, in the future when users can work 
        with multiple files we can start using it.
    */
%option noyywrap


    /* 
        These two options are doubled to get more detailed reports.
        Write performance report to stderr.
    */
%option perf-report perf-report

    /* Write statistics summary to stderr. */
%option verbose verbose

    /* Generate warning messages for mistakes. */
%option warn

    /* Maintain current line number in yylineno. */
%option yylineno


%option c++


    /* C items that can be used in the rules section */
%{
#include <stdio.h> 
#include <keywords.hxx>


int numIllegalCharacters;
int yycolno;


int isKeyWord(char * keyword);


    /* 
        This macro gets called the second we enter a rule,
        but before anything in that rule executes.

    #define YY_USER_ACTION                        \
    yylloc->first_line  = yylineno            \
    yylloc->last_line   = yylineno            \
    yylloc->first_column = yycolno             \
    yylloc->last_column  = yycolno + yyleng - 1\
    yycolno = yyleng                          
    */

%}

DIGIT      [0-9]
EXPONENT   [eE][-+]?{DIGIT}+


ID_START   [_a-zA-Z]
ID_AFTER   {ID_START}|{DIGIT}



    /*===================RULES===================*/
%% 


[-+*/%^?{}();,>]   {
  // Single character token.  Just return the char itself.
  return yytext[0];
}

[ \n\t\r]+ {
    /*Space do nothing */
}

[a-zA-Z] {
    return tok_NUM;
}


    /* ID or Keyword */
{ID_START}{ID_AFTER}*  {
    int result = isKeyWord(yytext);
    if(result) {
        return result;
    }

    return tok_ID;
}


    /* Gargabe */
.       {
    return tok_ILLEGAL;
}


%% 
    /*=================USER-CODE=================*/
#include <keywords.hxx>

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
  { "==",       tok_EQ       },
  { "!=",       tok_NE       },
  { ">=",       tok_NE       },
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

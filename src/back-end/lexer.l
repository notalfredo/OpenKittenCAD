    /*ttFlexLexer for running test*/


%option nodefault 

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
#include <iostream> 
#include <keywords.hxx>


int numIllegalCharacters;
int yycolno;


int isKeyWord(char * keyword);
void yypanic(char * errorMessage);

    /* 
        This macro gets called the second we enter a rule,
        but before anything in that rule executes.

        #define YY_USER_ACTION                           \
            yylloc->first_line   = lineno()              \
            yylloc->last_line    = lineno()              \
            yylloc->first_column = yycolno               \
            yylloc->last_column  = yycolno + YYLeng() - 1\
            yycolno += YYLeng()                          \
    */


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
    int result = isKeyWord(yytext);
    if(result) {
        return result;
    }

    return tok_ID;
}

    /* Gargabe */
.       {
    std::cout << "tok_ILLEGAL |" << YYText() << "| at ";
    std::cout << "line = " << lineno() << " column = " << yycolno << std::endl; 
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
  { "fn",       tok_FN       },
};

const int NUM_KEYWORDS = (sizeof(keywords) / sizeof(Keyword));

void yypanic(char * errorMessage) {
    std::cout << errorMessage << std::endl;
}

int isKeyWord(char * keyword){
    for(int index = 0; index < NUM_KEYWORDS; index++){
        if(!strcmp(keywords[index].keyWord, keyword)) {
            return keywords[index].token;
        }
    }
    return 0;
}

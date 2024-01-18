%option reentrant

/* Since we will be using Flex + Bison. */
/*     TODO: SETUP BISON                */
/*     IF WE SET UP THIS FLAG           */
/*     WITHOUT SETTING UP BISON FIRST   */
/*     WE GET A BUNCH OF  ERROS         */
/* %option bison-bridge bison-locations */

/* 
    Create the lexer exports header file with this name. 
    We want to expose the interface through which one can
    interact with the generated lexial analyzer
*/
%option header-file="lexer.yy.h"


/* We dont have an interactive session. */ 
%option never-interactive


/* We will handle the nodefault rule. */
%option nodefault

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
*/
#define YY_USER_ACTION                        \
    yylloc->first_line  = yylineno            \
    yylloc->last_line   = yylineno            \
    yyloc->first_column = yycolno             \
    yyloc->last_column  = yycolno + yyleng - 1\
    yycolno = yyleng                          

%}

DIGIT      [0-9]
EXPONENT   [eE][-+]?{DIGIT}+


ID_START   [_a-zA-Z]
ID_AFTER   {ID_START}|{DIGIT}



/*===================RULES===================*/
%% 


    /* ID or Keyword */
{ID_START}{ID_AFTER}*  {
    printf("Todo\n");
}

    /* Gargabe */
.       {
    printf("Todo\n");
}


%% 
/*=================USER-CODE=================*/
#include <keywords.hxx>

int isKeyWord(char * keyword){
    for(int index = 0; index < NUM_KEYWORDS; index++){
        if(strcmp(keywords[index].keyWord, keyword){
            return keywords[index].keyWord;
        }
    }
    return 0;
}

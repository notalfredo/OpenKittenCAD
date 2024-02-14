
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


typedef struct keyword {
    std::string name;
    int token;
} ktsKeywords;

ktsKeywords ktsKeywordsArr[] = {
    {"fn",       tok_FN},
    {"let",      tok_LET},

    {"break",    tok_BREAK},
    {"continue", tok_CONTINUE},
    {"return",   tok_RETURN},

    {"and",      tok_AND},
    {"or",       tok_OR},


    {"for",      tok_FOR},

    {"if",       tok_IF},
    {"elif",     tok_ELIF},
    {"else",     tok_ELSE},

    {"else",     tok_ELSE},
};

const int KEYWORD_ARR_SIZE = sizeof(ktsKeywordsArr) /sizeof(ktsKeywords);

int isKeyWord(std::string id) {
    for(int index = 0; index < KEYWORD_ARR_SIZE; index++){
        if( id == ktsKeywordsArr[index].name ){
            return ktsKeywordsArr[index].token;
        }
    }
    return 0; 
}
 
NodeType* nodeTypeFromChar(char* tokenText){
    if(!strcmp(tokenText, "number")){
        return newNodeType(num);
    }
    else if(!strcmp(tokenText, "shape")){
        return newNodeType(shape);
    }
    return newNodeType(_void);
}

%}

DIGIT       [0-9]
EXPONENT    [eE][-+]?{DIGIT}+


ALPHA       [_a-zA-Z]
ALPHA_NUM   {ALPHA}|{DIGIT}



    /*===================RULES===================*/
%% 

[-+*/%^?{}();,>=:]   {
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


"->" {
    flexString = yytext;
    return tok_ARROW;
}


"number" |
"shape"  |
"void"   {
    flexString = yytext;
    
    yylval->typeNode = nodeTypeFromChar(strdup(yytext));

    return tok_TYPE;
}

{DIGIT}+ |
({DIGIT}*(({DIGIT}\.)|(\.{DIGIT})){DIGIT}*){EXPONENT}? {
    flexString = yytext;

    yylval->numberNode = newNumberNode(strtod(yytext, NULL));    

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

    yylval->idNode = newIdentifierNode(strdup(yytext));
    return tok_ID;
}

    /* Gargabe */
.       {
    std::cout << "tok_ILLEGAL |" << yytext << "| at ";
    std::cout << "line = " << yylineno << " column = " << yycolumn << std::endl; 
    return tok_ILLEGAL;
}


%%

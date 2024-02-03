#include <cstdlib>
#include <iostream>
#include "tau.h"



#include "parser.tab.h"
#include "lexer.yy.h"



#include <unistd.h>


#include <iostream>  
#include <sstream>  
#include <string>    
#include <streambuf>
#include <fstream> 


extern std::string flexString;


TAU_MAIN()

char* tokToString(int tok) {
    switch (tok) {
        case tok_ILLEGAL: {
            return "tok_illegal";
        }
        case tok_EQ: {
            return "tok_eq";
        }
        case '<': {
            return "tok_lt";
        }
        case '>': {
            return "tok_gt";
        }
        case tok_LE: {
            return "tok_le";
        }
        case tok_GE: {
            return "tok_ge";
        }
        case tok_NE: {
            return "tok_ne";
        }
        case tok_AND: {
            return "tok_and";
        }
        case tok_OR: {
            return "tok_and";
        }    
        //todo
        //case tok_not: {
        //    return "tok_not";
        //}     
        case tok_ID: {
            return "tok_id";
        }
        case tok_FOR: {
            return "tok_for";
        }     
        case tok_REPEAT: {
            return "tok_repeat";
        }  
        case tok_UNTIL: {
            return "tok_until";
        }   
        case tok_WHILE: {
            return "tok_while";
        }  
        case tok_BREAK: {
            return "tok_break";
        }
        case tok_CONTINUE: {
            return "tok_continue";
        }
        case tok_IF: {
            return "tok_if";
        }
        case tok_ELIF: {
            return "tok_elif";
        }
        case tok_ELSE: {
            return "tok_else";
        }   
        case tok_LET: {
            return "tok_let";
        }    
        case tok_NUM: {
            return "tok_num";
        }    
        case tok_FN: {
            return "tok_fn";
        }    
        case tok_PIPE: {
            return "tok_pipe";
        }    
        case tok_ASSIGN: {
            return "tok_assign";
        }    
        case '(': {
            return "(";
        }    
        case ')': {
            return ")";
        }    
        case '{': {
            return "{";
        }    
        case '}': {
            return "}";
        }    
        case '=': {
            return "=";
        }    
        case ',': {
            return ",";
        }    
        case ';': {
            return ";";
        }    
        case '+': {
            return "+";
        }    
        case '-': {
            return "-";
        }    
        case '*': {
            return "*";
        }    
        case '%': {
            return "%";
        }    
        case '/': {
            return "/";
        }    
        default: {
            return "unknown token: |" ;
        }
    }
}


int tokEqToken(int tok, int expectedToken, int expectIllegal, const char* scannedText){
    switch (tok) {
        case tok_ILLEGAL: {
            std::cout << "tok_illegal: " << std::endl;
            if (expectIllegal) {
                return tok == expectedToken;
            }
            return 0;
        }
        case tok_EQ: {
            std::cout << "tok_eq" << std::endl;
            return tok == expectedToken;
        }
        case '<': {
            std::cout << "tok_lt" << std::endl;
            return tok == expectedToken;
        }
        case '>': {
            std::cout << "tok_gt" << std::endl;
            return tok == expectedToken;
        }
        case tok_LE: {
            std::cout << "tok_le" << std::endl;
            return tok == expectedToken;
        }
        case tok_GE: {
            std::cout << "tok_ge" << std::endl;
            return tok == expectedToken;
        }
        case tok_NE: {
            std::cout << "tok_ne" << std::endl;
            return tok == expectedToken;
        }
        case tok_AND: {
            std::cout << "tok_and" << std::endl;
            return tok == expectedToken;
        }
        case tok_OR: {
            std::cout << "tok_and" << std::endl;
            return tok == expectedToken;
        }    
        //todo
        //case tok_not: {
        //    std::cout << "tok_not" << std::endl;
        //    return tok == expectedToken;
        //}     
        case tok_ID: {
            std::cout << "tok_id" << std::endl;
            return tok == expectedToken;
        }
        case tok_FOR: {
            std::cout << "tok_for" << std::endl;
            return tok == expectedToken;
        }     
        case tok_REPEAT: {
            std::cout << "tok_repeat" << std::endl;
            return tok == expectedToken;
        }  
        case tok_UNTIL: {
            std::cout << "tok_until" << std::endl;
            return tok == expectedToken;
        }   
        case tok_WHILE: {
            std::cout << "tok_while" << std::endl;
            return tok == expectedToken;
        }  
        case tok_BREAK: {
            std::cout << "tok_break" << std::endl;
            return tok == expectedToken;
        }
        case tok_CONTINUE: {
            std::cout << "tok_continue" << std::endl;
            return tok == expectedToken;
        }
        case tok_IF: {
            std::cout << "tok_if" << std::endl;
            return tok == expectedToken;
        }
        case tok_ELIF: {
            std::cout << "tok_elif" << std::endl;
            return tok == expectedToken;
        }
        case tok_ELSE: {
            std::cout << "tok_else" << std::endl;
            return tok == expectedToken;
        }   
        case tok_LET: {
            std::cout << "tok_let" << std::endl;
            return tok == expectedToken;
        }    
        case tok_NUM: {
            std::cout << "tok_num" << std::endl;
            return tok == expectedToken;
        }    
        case tok_FN: {
            std::cout << "tok_fn" << std::endl;
            return tok == expectedToken;
        }    
        case tok_PIPE: {
            std::cout << "tok_pipe" << std::endl;
            return tok == expectedToken;
        }    
        case tok_ASSIGN: {
            std::cout << "tok_assign" << std::endl;
            return tok == expectedToken;
        }    
        case '(': {
            std::cout << "(" << std::endl;
            return tok == expectedToken;
        }    
        case ')': {
            std::cout << ")" << std::endl;
            return tok == expectedToken;
        }    
        case '{': {
            std::cout << "{" << std::endl;
            return tok == expectedToken;
        }    
        case '}': {
            std::cout << "}" << std::endl;
            return tok == expectedToken;
        }    
        case '=': {
            std::cout << "=" << std::endl;
            return tok == expectedToken;
        }    
        case ',': {
            std::cout << "," << std::endl;
            return tok == expectedToken;
        }    
        case ';': {
            std::cout << ";" << std::endl;
            return tok == expectedToken;
        }    
        case '+': {
            std::cout << "+" << std::endl;
            return tok == expectedToken;
        }    
        case '-': {
            std::cout << "-" << std::endl;
            return tok == expectedToken;
        }    
        case '*': {
            std::cout << "*" << std::endl;
            return tok == expectedToken;
        }    
        case '%': {
            std::cout << "%" << std::endl;
            return tok == expectedToken;
        }    
        case '/': {
            std::cout << "/" << std::endl;
            return tok == expectedToken;
        }    
        default: {
            std::cout << "unkown token: |" << scannedText << "|" << std::endl;
            return 0;
        }
    }
    return 0;
}

const std::string tempStr = TEST_DIR + std::string("/tests/input_tests");
const char* dirLoc = tempStr.c_str();



    
TEST(lexer, testOne) { 
    int testArr[] { 
        tok_FN, tok_ID, '(', ')', '{', tok_LET, tok_ID, 
        tok_ASSIGN, tok_ID, '(', tok_ID, '=', tok_NUM, ')',
        tok_PIPE, tok_ID, '(', '%', ',', tok_NUM, ',', tok_NUM, ',',
        tok_NUM, ')', tok_PIPE, tok_ID, '(', '%', ',', tok_NUM, ')', ';',
        tok_LET, tok_ID, tok_ASSIGN, tok_ID, '(', tok_ID, '=',
        tok_NUM, ',', tok_ID, '=', tok_NUM, ')', ';', tok_ID,
        '(', tok_ID, ',', tok_ID, ')', '}',
    };

    FILE *srcFP       = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/input.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file ");
    }

    yyscan_t scanner;
    yylex_init( &scanner );

    yyrestart( srcFP, scanner );

    yyset_lineno( 1, scanner );

    YYSTYPE yylval_param;
    YYLTYPE yylloc_param;

    int index = 0; int expectIllegal = 0; int tok;

    while ( ( tok = yylex(&yylval_param, &yylloc_param ,scanner) ) ) {

        std::cout << "Current text: \"" << flexString << "\"" << std::endl;

        if(index > (sizeof(testArr) / sizeof(int))) {
            CHECK(0, "ABORT: len(index) > len(testOneArr)"); 
            exit(1);
        }

        //std::string l = str(tokToString(tok);

        //std::string text = ">>>>> left : " << l << " " << ">>>>> right: " << tokToString(testArr[index]);

                        
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        ),
                ">>>>>>>>>>>"
        );
        index += 1;
    }
}



TEST(lexer, testTwo) { 
    int testArr[] { 
        tok_FN, tok_ID, '(', ')', '{', tok_LET, tok_ID, 
        tok_ASSIGN, tok_ID, '(', tok_ID, '=', tok_NUM, ')',
        tok_PIPE, tok_ID, '(', '%', ',', tok_NUM, ',', tok_NUM, ',',
        tok_NUM, ')', ';', tok_LET, tok_ID, tok_ASSIGN, 
        tok_ID, '(', tok_ID, '=', tok_NUM, ')', ';',
        tok_ID, '(', tok_ID, '(', tok_ID, ',', tok_ID,
        ')', ',', tok_ID, '(', tok_ID, '(', tok_NUM,
        '+', tok_NUM, ',', tok_NUM, '-', tok_NUM, ')', ',',
        tok_ID, '(', tok_NUM, '*', tok_NUM, ',', tok_NUM,
        '%', tok_NUM, ',', tok_NUM, '/', tok_NUM, ')',
        ')', ')', '}',
    };


    FILE *srcFP       = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/inputTwo.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file ");
    }

    yyscan_t scanner;
    yylex_init( &scanner );

    yyrestart( srcFP, scanner );

    yyset_lineno( 1, scanner );


    YYSTYPE yylval_param;
    YYLTYPE yylloc_param;

    int index = 0; int expectIllegal = 0; int tok;

    while ( ( tok = yylex(&yylval_param, &yylloc_param ,scanner) ) ) {

        std::cout << "Current text: \"" << flexString << "\"" << std::endl;

        if(index > (sizeof(testArr) / sizeof(int))) {
            CHECK(0, "ABORT: len(index) > len(testOneArr)"); 
            exit(1);
        }
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        ));
        index += 1;
    }
}

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

std::string tokToString(int tok) {
    switch (tok) {
        case tok_ILLEGAL: {
            return "tok_ILLEGAL";
        }
        case tok_EQ: {
            return "tok_EQ";
        }
        case '<': {
            return "tok_LT";
        }
        case '>': {
            return "tok_GT";
        }
        case tok_LE: {
            return "tok_LE";
        }
        case tok_GE: {
            return "tok_GE";
        }
        case tok_NE: {
            return "tok_NE";
        }
        case tok_AND: {
            return "tok_AND";
        }
        case tok_OR: {
            return "tok_OR";
        }    
        case tok_ID: {
            return "tok_ID";
        }
        case tok_FOR: {
            return "tok_FOR";
        }     
        case tok_REPEAT: {
            return "tok_REPEAT";
        }  
        case tok_UNTIL: {
            return "tok_UNTIL";
        }   
        case tok_WHILE: {
            return "tok_WHILE";
        }  
        case tok_BREAK: {
            return "tok_BREAK";
        }
        case tok_CONTINUE: {
            return "tok_CONTINUE";
        }
        case tok_IF: {
            return "tok_IF";
        }
        case tok_ELIF: {
            return "tok_ELIF";
        }
        case tok_ELSE: {
            return "tok_ELSE";
        }   
        case tok_LET: {
            return "tok_LET";
        }    
        case tok_NUM: {
            return "tok_NUM";
        }    
        case tok_FN: {
            return "tok_FN";
        }    
        case tok_PIPE: {
            return "tok_PIPE";
        }    
        case tok_ASSIGN: {
            return "tok_ASSIGN";
        }    
        case tok_TYPE: {
            return "tok_ASSIGN";
        }    
        case tok_RETURN: {
            return "tok_RETURN";
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
        case ':': {
            return ":";
        }    
        default: {
            return "unknown token: |" ;
        }
    }
}


int tokEqToken(int tok, int expectedToken, int expectIllegal, const char* scannedText){
    switch (tok) {
        case tok_ILLEGAL: {
            if (expectIllegal) {
                return tok == expectedToken;
            }
            return 0;
        }
        case tok_EQ: {
            return tok == expectedToken;
        }
        case '<': {
            return tok == expectedToken;
        }
        case '>': {
            return tok == expectedToken;
        }
        case tok_LE: {
            return tok == expectedToken;
        }
        case tok_GE: {
            return tok == expectedToken;
        }
        case tok_NE: {
            return tok == expectedToken;
        }
        case tok_AND: {
            return tok == expectedToken;
        }
        case tok_OR: {
            return tok == expectedToken;
        }    
        case tok_ID: {
            return tok == expectedToken;
        }
        case tok_FOR: {
            return tok == expectedToken;
        }     
        case tok_REPEAT: {
            return tok == expectedToken;
        }  
        case tok_UNTIL: {
            return tok == expectedToken;
        }   
        case tok_WHILE: {
            return tok == expectedToken;
        }  
        case tok_BREAK: {
            return tok == expectedToken;
        }
        case tok_CONTINUE: {
            return tok == expectedToken;
        }
        case tok_IF: {
            return tok == expectedToken;
        }
        case tok_ELIF: {
            return tok == expectedToken;
        }
        case tok_ELSE: {
            return tok == expectedToken;
        }   
        case tok_LET: {
            return tok == expectedToken;
        }    
        case tok_NUM: {
            return tok == expectedToken;
        }    
        case tok_FN: {
            return tok == expectedToken;
        }    
        case tok_PIPE: {
            return tok == expectedToken;
        }    
        case tok_ASSIGN: {
            return tok == expectedToken;
        }    
        case tok_TYPE: {
            return tok == expectedToken;
        }    
        case tok_RETURN: {
            return tok == expectedToken;
        }    
        case '(': {
            return tok == expectedToken;
        }    
        case ')': {
            return tok == expectedToken;
        }    
        case '{': {
            return tok == expectedToken;
        }    
        case '}': {
            return tok == expectedToken;
        }    
        case '=': {
            return tok == expectedToken;
        }    
        case ',': {
            return tok == expectedToken;
        }    
        case ';': {
            return tok == expectedToken;
        }    
        case '+': {
            return tok == expectedToken;
        }    
        case '-': {
            return tok == expectedToken;
        }    
        case '*': {
            return tok == expectedToken;
        }    
        case '%': {
            return tok == expectedToken;
        }    
        case '/': {
            return tok == expectedToken;
        }    
        case ':': {
            return tok == expectedToken;
        }    
        default: {
            std::cout << "unkown token: >>" << scannedText << "<<" << std::endl;
            return 0;
        }
    }
    return 0;
}

const std::string tempStr = PROJ_DIR + std::string("/tests/input_tests");
const char* dirLoc = tempStr.c_str();


TEST(lexer, testOne) { 
    CHECK_EQ(0, countAllocatedNodes());
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

        std::string receivedTokenStr = tokToString(tok);
        std::string expectedTokenStr = tokToString(testArr[index]);

        std::string  errMessage = ">>>>>Received: " + receivedTokenStr  + " Expected: " + expectedTokenStr;

        
        std::cout << errMessage << std::endl;
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        )
        );
        index += 1;
    }

    yylex_destroy(scanner);
    freeAllNodes();
    fclose(srcFP);
    CHECK_EQ(0, countAllocatedNodes());
}


TEST(lexer, testTwo) { 
    CHECK_EQ(0, countAllocatedNodes());
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

        std::string receivedTokenStr = tokToString(tok);
        std::string expectedTokenStr = tokToString(testArr[index]);

        std::string  errMessage = ">>>>>Received: " + receivedTokenStr  + " Expected: " + expectedTokenStr;

        
        std::cout << errMessage << std::endl;
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        )
        );
        index += 1;
    }

    countAllocatedNodes();
    freeAllNodes();
    yylex_destroy(scanner);
    fclose(srcFP);
    CHECK_EQ(0, countAllocatedNodes());
}


TEST(lexer, testThree) { 
    CHECK_EQ(0, countAllocatedNodes());
    int testArr[] { 
        tok_FN, tok_ID, '(', tok_ID, ':', tok_TYPE, ',', 
        tok_ID, ':', tok_TYPE, ')', '{', tok_RETURN, tok_ID, 
        '+', tok_ID, ';', '}', tok_FN, tok_ID, '(', ')',
        '{', tok_LET, tok_ID, tok_ASSIGN, tok_ID, '(',
        tok_NUM, ',', tok_NUM, ')', ';', '}'
    };


    FILE *srcFP       = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/inputThree.kts",
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

        std::string receivedTokenStr = tokToString(tok);
        std::string expectedTokenStr = tokToString(testArr[index]);

        std::string  errMessage = ">>>>>Received: " + receivedTokenStr  + " Expected: " + expectedTokenStr;

        
        std::cout << errMessage << std::endl;
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        )
        );
        index += 1;
    }

    freeAllNodes();
    yylex_destroy(scanner);
    fclose(srcFP);
    CHECK_EQ(0, countAllocatedNodes());
}




TEST(lexer, testFour) { 
    CHECK_EQ(0, countAllocatedNodes());
    int testArr[] { 
        tok_FN, tok_ID, '(', ')', '{', '}', ';'
    };


    FILE *srcFP  = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/inputFour.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        fprintf(stderr, "Unable to open file\n");
        return;
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

        std::string receivedTokenStr = tokToString(tok);
        std::string expectedTokenStr = tokToString(testArr[index]);

        std::string  errMessage = ">>>>>Received: " + receivedTokenStr  + " Expected: " + expectedTokenStr;

        
        std::cout << errMessage << std::endl;
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        )
        );
        index += 1;
    }

    freeAllNodes();
    yylex_destroy(scanner);
    fclose(srcFP);
    CHECK_EQ(0, countAllocatedNodes());
}

TEST(lexer, testFive) { 
    CHECK_EQ(0, countAllocatedNodes());
    int testArr[] { 
        '{', tok_LET, tok_ID, ':', tok_TYPE, tok_ASSIGN, tok_NUM, '+', 
        tok_NUM, ';', tok_ID, '(', tok_ID, ')', ';',
        '}', ';'
    };


    FILE *srcFP  = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/inputFive.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        fprintf(stderr, "Unable to open file\n");
        return;
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

        std::string receivedTokenStr = tokToString(tok);
        std::string expectedTokenStr = tokToString(testArr[index]);

        std::string  errMessage = ">>>>>Received: " + receivedTokenStr  + " Expected: " + expectedTokenStr;

        
        std::cout << errMessage << std::endl;
        CHECK(tokEqToken(
                    tok,
                    testArr[index],
                    0,
                    flexString.c_str()
        )
        );
        index += 1;
    }

    freeAllNodes();
    yylex_destroy(scanner);
    fclose(srcFP);
    CHECK_EQ(0, countAllocatedNodes());
}


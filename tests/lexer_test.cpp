#include <cstdlib>
#include <iostream>

#include "keywords.hxx"
#include "lexer.yy.h"


#include "tau.h"
#include <unistd.h>


#include <iostream>  
#include <sstream>  
#include <string>    
#include <streambuf>
#include <fstream> 




TAU_MAIN()

int tokEqToken(int tok, int expectedToken, int expectIllegal, const char* scannedText){
    switch (tok) {
        case tok_ILLEGAL: {
            std::cout << "tok_ILLEGAL: " << std::endl;
            if (expectIllegal) {
                return tok == expectedToken;
            }
            return 0;
        }
        case tok_EQ: {
            std::cout << "tok_EQ" << std::endl;
            return tok == expectedToken;
        }
        case tok_LT: {
            std::cout << "tok_LT" << std::endl;
            return tok == expectedToken;
        }
        case tok_GT: {
            std::cout << "tok_GT" << std::endl;
            return tok == expectedToken;
        }
        case tok_LE: {
            std::cout << "tok_LE" << std::endl;
            return tok == expectedToken;
        }
        case tok_GE: {
            std::cout << "tok_GE" << std::endl;
            return tok == expectedToken;
        }
        case tok_NE: {
            std::cout << "tok_NE" << std::endl;
            return tok == expectedToken;
        }
        case tok_AND: {
            std::cout << "tok_AND" << std::endl;
            return tok == expectedToken;
        }
        case tok_OR: {
            std::cout << "tok_AND" << std::endl;
            return tok == expectedToken;
        }    
        case tok_NOT: {
            std::cout << "tok_NOT" << std::endl;
            return tok == expectedToken;
        }     
        case tok_ID: {
            std::cout << "tok_ID" << std::endl;
            return tok == expectedToken;
        }
        case tok_FOR: {
            std::cout << "tok_FOR" << std::endl;
            return tok == expectedToken;
        }     
        case tok_REPEAT: {
            std::cout << "tok_REPEAT" << std::endl;
            return tok == expectedToken;
        }  
        case tok_UNTIL: {
            std::cout << "tok_UNTIL" << std::endl;
            return tok == expectedToken;
        }   
        case tok_WHILE: {
            std::cout << "tok_WHILE" << std::endl;
            return tok == expectedToken;
        }  
        case tok_BREAK: {
            std::cout << "tok_BREAK" << std::endl;
            return tok == expectedToken;
        }
        case tok_CONTINUE: {
            std::cout << "tok_CONTINUE" << std::endl;
            return tok == expectedToken;
        }
        case tok_IF: {
            std::cout << "tok_IF" << std::endl;
            return tok == expectedToken;
        }
        case tok_ELIF: {
            std::cout << "tok_ELIF" << std::endl;
            return tok == expectedToken;
        }
        case tok_ELSE: {
            std::cout << "tok_ELSE" << std::endl;
            return tok == expectedToken;
        }   
        case tok_LET: {
            std::cout << "tok_LET" << std::endl;
            return tok == expectedToken;
        }    
        case tok_NUM: {
            std::cout << "tok_NUM" << std::endl;
            return tok == expectedToken;
        }    
        case tok_FN: {
            std::cout << "tok_FN" << std::endl;
            return tok == expectedToken;
        }    
        case tok_PIPE: {
            std::cout << "tok_PIPE" << std::endl;
            return tok == expectedToken;
        }    
        case tok_ASSIGN: {
            std::cout << "tok_ASSIGN" << std::endl;
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
            std::cout << "UNKOWN TOKEN: |" << scannedText << "|" << std::endl;
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
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/sam.txt",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file ");
    }

    yyscan_t scanner;
    yylex_init( &scanner );

    //yyrestart( srcFP, scanner );

    //yyset_lineno( 1, scanner );

    int tok;



    //int index = 0; int expectIllegal = 0; int tok;

    //while ( ( tok = myTest.yylex() ) ) {
    //    std::cout << "Current text: \"" << myTest.YYText() << "\"" << std::endl;

    //    if(index > (sizeof(testArr) / sizeof(int))) {
    //        CHECK(0, "ABORT: len(index) > len(testOneArr)"); 
    //        exit(1);
    //    }
    //    CHECK(tokEqToken(
    //                tok,
    //                testArr[index],
    //                0,
    //                myTest.YYText()
    //    ));
    //    index += 1;
    //}
}



TEST(lexer, testTwo) { 
    //std::ifstream inputStreamTwo(testInputDir + "/inputTwo.kts");
    //myTest.switch_streams(inputStreamTwo, outputStream);
    //int testArr[] { 
    //    tok_FN, tok_ID, '(', ')', '{', tok_LET, tok_ID, 
    //    tok_ASSIGN, tok_ID, '(', tok_ID, '=', tok_NUM, ')',
    //    tok_PIPE, tok_ID, '(', '%', ',', tok_NUM, ',', tok_NUM, ',',
    //    tok_NUM, ')', ';', tok_LET, tok_ID, tok_ASSIGN, 
    //    tok_ID, '(', tok_ID, '=', tok_NUM, ')', ';',
    //    tok_ID, '(', tok_ID, '(', tok_ID, ',', tok_ID,
    //    ')', ',', tok_ID, '(', tok_ID, '(', tok_NUM,
    //    '+', tok_NUM, ',', tok_NUM, '-', tok_NUM, ')', ',',
    //    tok_ID, '(', tok_NUM, '*', tok_NUM, ',', tok_NUM,
    //    '%', tok_NUM, ',', tok_NUM, '/', tok_NUM, ')',
    //    ')', ')', '}',
    //};


    //int index = 0; int expectIllegal = 0; int tok;

    //while ( ( tok = myTest.yylex() ) ) {
    //    std::cout << "Current text: \"" << myTest.YYText() << "\"" << std::endl;
    //    if(index > (sizeof(testArr) / sizeof(int))) {
    //        CHECK(0, "ABORT: len(index) > len(testOneArr)"); 
    //        exit(1);
    //    }
    //    CHECK(tokEqToken(
    //                tok,
    //                testArr[index],
    //                0,
    //                myTest.YYText()
    //    ));
    //    index += 1;
    //}
}

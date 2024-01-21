#include <cstdlib>
#include <iostream>
#include <keywords.hxx>

#include "tau.h"
#include <unistd.h>


#include <iostream>  
#include <sstream>  
#include <string>    
#include <streambuf>
#include <fstream> 


#include <FlexLexer.h>


TAU_MAIN()

int tokEqToken(int tok, int expectedToken, int expectIllegal){
    switch (tok) {
        case tok_ILLEGAL: {
            std::cout << "tok_ILLEGAL" << std::endl;
            if (expectIllegal) {
                return tok == expectedToken;
            }
            else {
                return 0;
            }
            break;
        }
        case tok_EQ: {
            std::cout << "tok_EQ" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_LT: {
            std::cout << "tok_LT" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_GT: {
            std::cout << "tok_GT" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_LE: {
            std::cout << "tok_LE" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_GE: {
            std::cout << "tok_GE" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_NE: {
            std::cout << "tok_NE" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_AND: {
            std::cout << "tok_AND" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_OR: {
            std::cout << "tok_AND" << std::endl;
            return tok == expectedToken;
            break;
        }    
        case tok_NOT: {
            std::cout << "tok_NOT" << std::endl;
            return tok == expectedToken;
            break;
        }     
        case tok_UNION: {
            std::cout << "tok_UNION" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_DIFFERENCE: {
            std::cout << "tok_DIFFERENCE" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_INTERSECTION: {
            std::cout << "tok_INTERSECTION" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_ID: {
            std::cout << "tok_ID" << std::endl;
            return tok == expectedToken;
            break;
        }

        case tok_FOR: {
            std::cout << "tok_FOR" << std::endl;
            return tok == expectedToken;
            break;
        }     
        case tok_REPEAT: {
            std::cout << "tok_REPEAT" << std::endl;
            return tok == expectedToken;
            break;
        }  
        case tok_UNTIL: {
            std::cout << "tok_UNTIL" << std::endl;
            return tok == expectedToken;
            break;
        }   
        case tok_WHILE: {
            std::cout << "tok_WHILE" << std::endl;
            return tok == expectedToken;
            break;
        }  
        case tok_BREAK: {
            std::cout << "tok_BREAK" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_CONTINUE: {
            std::cout << "tok_CONTINUE" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_IF: {
            std::cout << "tok_IF" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_ELIF: {
            std::cout << "tok_ELIF" << std::endl;
            return tok == expectedToken;
            break;
        }
        case tok_ELSE: {
            std::cout << "tok_ELSE" << std::endl;
            return tok == expectedToken;
            break;
        }   
        case tok_LET: {
            std::cout << "tok_LET" << std::endl;
            return tok == expectedToken;
            break;
        }    
        case tok_NUM: {
            std::cout << "tok_ID" << std::endl;
            return tok == expectedToken;
            break;
        }    
        default: {
            std::cout << "HOW DID YOU END UP HERE ?" << std::endl;
            return 0;
            break;
        }
    }
    return 0;
}

    
TEST(lexer, input) { 
    std::string stringvalues = "TOKEN TEST 123";
    std::istringstream inputStringStream(stringvalues);
    std::istream& inputStream = inputStringStream;
    std::ofstream outputStream("test.txt");

    yyFlexLexer myTest(inputStream, outputStream);

    tokens testArr[] {
        tok_ID,
        tok_ID,
        tok_NUM
    };

    int index, expectIllegal = 0; 
    int tok;
    while ( ( tok = myTest.yylex() ) ) {
        std::cout << tok << std::endl;
        //tokEqToken(tok, testArr[index], 0);
        //CHECK(tokEqToken(tok, testArr[index], 0));
    }
}

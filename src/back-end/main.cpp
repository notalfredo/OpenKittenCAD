#include <string.h>

//#include <FlexLexer.h>
#include "parser.tab.h"
#include "lexer.yy.h"


#include <iostream>  
#include <sstream>  
#include <string>    
#include <streambuf>
#include <fstream> 

int main() {
    FILE *srcFP       = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/sam.txt",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file ");
    }

    yyscan_t scanner;
    yylex_init( &scanner );

    yyrestart( srcFP, scanner );

    yyset_lineno( 1, scanner );

    int tok;


    //while ( (tok = yylex(scanner)) ) {
    //    std::cout << tok << std::endl;
    //}
}

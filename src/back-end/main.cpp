#include <string.h>

#include "parser.tab.h"
#include "lexer.yy.h"


#include <iostream>  
#include <sstream>  
#include <string>    
#include <streambuf>
#include <fstream> 

int main() {
    FILE *srcFP       = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/input.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file\n");
    }

    yyscan_t scanner;
    yylex_init( &scanner );

    yyrestart( srcFP, scanner );

    yyset_lineno( 1, scanner );

    int tok;

    void *result = NULL;
    int parseState = yyparse( scanner, &result );
 

    //while ( (tok = yylex(scanner)) ) {
    //    std::cout << tok << std::endl;
    //}
}

#include <cstdlib>
#include <iostream>
#include "tau.h"



#include "parser.tab.h"
#include "lexer.yy.h"



#include <unistd.h>



extern void programToJson(std::vector<NodeStatement*>* head, const char* fileLocation);


TAU_MAIN()


TEST(parser, testOne) { 

    std::string outputPath =  "/home/alfredo/repos/OpenKittenCad/tests/json/output.json";


    FILE *srcFP  = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/inputFour.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file exiting...");
        return;
    }

    yyscan_t scanner;
    yylex_init( &scanner );
    yyrestart( srcFP, scanner );
    yyset_lineno( 1, scanner );


    void *result = NULL;
    int parseState = yyparse( scanner, &result );


    if(parseState != 0){
        std::cout << "PARSING FAILED" << std::endl; 
    }else {
        programToJson((std::vector<NodeStatement*>*) result, outputPath.c_str());
    }

    yylex_destroy(scanner);
}


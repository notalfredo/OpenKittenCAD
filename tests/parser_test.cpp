#include <cstdlib>
#include <iostream>
#include "tau.h"

#include <node.hxx>


#include "parser.tab.h"
#include "lexer.yy.h"



#include <unistd.h>


extern int yydebug;

extern void programToJson(NodeStmtList* head, const char* fileLocation);


TAU_MAIN()


TEST(parser, testOne) { 
    std::string outputPath =  "/home/alfredo/repos/OpenKittenCad/tests/json/parserOutputOne.json";


    FILE *srcFP  = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/parserInputOne.kts",
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


    //yydebug = 1;
    void *result = NULL;
    int parseState = yyparse( scanner, &result );
    
    if(parseState != 0){
        std::cout << "PARSING FAILED" << std::endl; 
        CHECK(0);
    }else {
        programToJson((NodeStmtList*)result, outputPath.c_str());
    }

    freeAllNodes();
    yylex_destroy(scanner);
    fclose(srcFP);
    CHECK_EQ(0, countAllocatedNodes());
}

TEST(parser, testTwo) { 
    std::string outputPath =  "/home/alfredo/repos/OpenKittenCad/tests/json/parserOutputTwo.json";


    FILE *srcFP  = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/parserInputTwo.kts",
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


    //yydebug = 1;
    void *result = NULL;
    int parseState = yyparse( scanner, &result );
    
    if(parseState != 0){
        std::cout << "PARSING FAILED" << std::endl; 
        CHECK(0);
    }else {
        programToJson((NodeStmtList*)result, outputPath.c_str());
    }

    yylex_destroy(scanner);
    freeAllNodes();
    fclose(srcFP);
}



TEST(parser, testThree) { 
    //std::string outputPath =  "/home/alfredo/repos/OpenKittenCad/tests/json/parserOutputTwo.json";


    FILE *srcFP  = fopen( 
    "/home/alfredo/repos/OpenKittenCad/tests/input_tests/parserInputThree.kts",
    "r"
    );
    if ( srcFP == NULL ) {
        printf( "Unable to open file exiting...");
        return;
    }
    fprintf(stderr, "================\n");

    yyscan_t scanner;
    yylex_init( &scanner );
    yyrestart( srcFP, scanner );
    yyset_lineno( 1, scanner );


    yydebug = 1;
    void *result = NULL;
    int parseState = yyparse( scanner, &result );
    
    if(parseState != 0){
        std::cout << "PARSING FAILED" << std::endl; 
        CHECK(0);
    }else {
        //programToJson((NodeStmtList*)result, outputPath.c_str());
    }

    yylex_destroy(scanner);
    freeAllNodes();
    fclose(srcFP);
}

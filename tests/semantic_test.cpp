#include "node.hxx"
#include "tau.h"
#include "parser.tab.h"
#include "lexer.yy.h"
#include <cstdio>

extern void semantic(NodeStmtList* head);
extern int yydebug;


TAU_MAIN()


TEST(semantic, testOne) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputOne.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }


    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testTwo) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputTwo.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }



    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testThree) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputThree.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }

    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testFour) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputFour.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }

    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testFive) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputFive.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }

    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testSix) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputSix.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }


    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}



TEST(semantic, testSeven) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputSeven.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }


    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testEight) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputEight.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }


    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}


TEST(semantic, testNine) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/semanticInputNine.kts", "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
    //yydebug = 1;
    yyscan_t scanner;
    yylex_init(&scanner);
    yyrestart(filePtr, scanner);
    yyset_lineno(1, scanner);

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        fprintf(stderr, "Error parsing exiting...\n");
        CHECK(0);
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result);
        CHECK(1);
    }


    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}

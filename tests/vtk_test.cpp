#include "node.hxx"
#include "tau.h"
#include "parser.tab.h"
#include "lexer.yy.h"
#include <cstdio>

extern void semantic(NodeStmtList* head, int displayVTK = 1);
extern int yydebug;


TAU_MAIN()


TEST(VTK, testOne) {
    
    FILE* filePtr = fopen("/home/alfredo/repos/OpenKittenCad/tests/input_tests/vtkInputFive.kts", "r");
    
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
    }
    else {
        fprintf(stdout, "Now performing semantic analysis\n\n");        
        semantic((NodeStmtList*)result, 1);
    }


    yylex_destroy(scanner);
    freeAllNodes();
    fclose(filePtr);
}

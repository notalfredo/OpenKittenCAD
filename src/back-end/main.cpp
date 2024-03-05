#include "node.hxx"
#include "parser.tab.h"
#include "lexer.yy.h"
#include <cstdio>

extern void semantic(NodeStmtList* head, int displayVTK = 1);

int main(int argc, char** argv) {

    FILE* filePtr = fopen(argv[1], "r");
    
    if(!filePtr){
        fprintf(stderr, "Error opening file semantic testOne\n");
        fclose(filePtr); 
    }

     
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

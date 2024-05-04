#include "node.hxx"
#include "frontEndEntryPoint.hxx"
#include "parser.tab.h"
#include "lexer.yy.h"
#include <cstdio>



extern int numIllegalCharacters;
extern void semantic(NodeStmtList* head);
extern shapesVector getShapes();


programResponse* newProgramResponse(errorStatus status, const char* message)
{
    programResponse* prog = (programResponse*)calloc(1, sizeof(programResponse));     

    prog->message = strdup(message);
    prog->status = status;

    
    if(status == success){
        prog->vec = getShapes();
    }


    return prog;
}



programResponse* frontEndEntryPoint(const char* program, programType progType) {

    FILE* filePtr = NULL;
     
    yyscan_t scanner;
    yylex_init(&scanner);

    if(progType == file){
        std::cout << "Opening file" << std::endl;


        FILE* filePtr = fopen(program, "r");

        if(!filePtr){
            fclose(filePtr); 
            return newProgramResponse(other_error, "Unable to open file");
        }

        yyrestart(filePtr, scanner);
    }
    else if(progType == string){
        std::cout << "Using string" << std::endl;

        yy_scan_string(program, scanner);

        std::cout << "yy_scan_string" << std::endl;
    }

    yyset_lineno(1, scanner);

    if(numIllegalCharacters){
        //TODO RETURN WHAT TOKENS WE DID NOT LIKE
        return newProgramResponse(lexical_error, "Unable to tokanize\n");
    }

    void *result = NULL;
    int parseState = yyparse(scanner, &result);

    
    if(parseState != 0){
        //TODO RETURN WHY WE ARE UANBLE TO PARSE
        return newProgramResponse(parser_error, "Unable to parse program");
    }
    else {
        //TODO RETURN WHY WE FAILED (IF WE FAILED)
        
        fprintf(stdout, "Now performing semantic analysis\n\n"); 
        semantic((NodeStmtList*)result);
    }


    yylex_destroy(scanner);
    
    /*
        freeAllNodes(); // TODO DO THIS AFTER WE CLOSE PROGRAM
        fclose(filePtr);
    */

    return newProgramResponse(success, "success");
}

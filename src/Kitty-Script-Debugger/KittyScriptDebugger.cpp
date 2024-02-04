#include "node.hxx"
#include "DebuggerHelperFunc.hxx"

#include <stdio.h>
#include <vector>
#include <iostream>


/*Private variables and functions to this mmodule*/
static FILE* _fptr = NULL;

static void _stmtNodeVecToJson(std::vector<NodeStatement*>* stmtVec, int blockLevel){
    for(int index = 0; index < (*stmtVec).size(); index++){
        if(!(*stmtVec)[index]){
            std::cout << "ERROR ENCOUNTERED NULL" << std::endl;
            return;
        }
        std::string nodeName = nodeTypeToString((*stmtVec)[index]->nodeType);
        fprintf(_fptr, nodeName.c_str());
    }
}



/* Entry point to this module */
void programToJson(
    std::vector<NodeStatement*>* head,
    const char* fileLocation){
     
    _fptr = fopen(fileLocation, "w+");
    if(!_fptr){
        std::cout << "ERROR OPENING FILE IN WRITE MODE FILE: " << fileLocation << std::endl;
        return;
    }


    _stmtNodeVecToJson(head, 0);
}

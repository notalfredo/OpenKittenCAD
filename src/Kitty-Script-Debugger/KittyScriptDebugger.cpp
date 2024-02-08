#include "node.hxx"
#include "DebuggerHelperFunc.hxx"

#include <stdio.h>
#include <vector>
#include <iostream>


/*Private variables and functions to this mmodule*/
static FILE* _fptr = NULL;

static void _stmtNodeVecToJson(NodeStmtList* stmtVec, int blockLevel){

    int stmtVecSize = getStmtListSize(stmtVec);
    for(int index = 0; index < stmtVecSize; index++){

        NodeStatement* currentNode = indexStmtList(stmtVec, index);
        if( !currentNode ){
            std::cout << "ERROR ENCOUNTERED NULL" << std::endl;
            return;
        }
        else {
            std::cout << "NODE TYPE " << currentNode->nodeType << std::endl;
        }
        //std::string nodeName = nodeTypeToString((*stmtVec)[index]->nodeType);
        //fprintf(_fptr, nodeName.c_str());
    }
}


/* Entry point to this module */
void programToJson(
    NodeStmtList* head,
    const char* fileLocation){
     
    _fptr = fopen(fileLocation, "w+");
    if(!_fptr){
        std::cout << "ERROR OPENING FILE IN WRITE MODE FILE: " << fileLocation << std::endl;
        return;
    }


    _stmtNodeVecToJson(head, 0);
}

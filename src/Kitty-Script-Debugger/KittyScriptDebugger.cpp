#include "node.hxx"
#include <stdio.h>
#include <vector>
#include <iostream>

    /*Private file pointer local to this module only*/
static FILE* _fptr = NULL;

void stmtNodeVecToJson(std::vector<NodeStatement*>* stmtVec, int blockLevel){
    for(int index = 0; index < (*stmtVec).size(); index++){
        std::string nodeName = nodeTypeToString((*stmtVec)[index]->nodeType);
        fprintf(_fptr, nodeName.c_str());
    }
}


void programToJson(
    std::vector<NodeStatement*>* head,
    char* fileLocation){
     
    _fptr = fopen(fileLocation, "w+");
    if(!_fptr){
        std::cout << "ERROR OPENING FILE IN WRITE MODE" << std::endl;
        return;
    }

    stmtNodeVecToJson(head, 0);
}

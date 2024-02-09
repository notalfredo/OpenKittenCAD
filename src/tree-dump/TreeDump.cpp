#include "node.hxx"
#include "TreeDumpHelper.hxx"

#include <stdio.h>
#include <vector>
#include <iostream>


/*Private variables and functions to this mmodule*/
static FILE* _fptr = NULL;


void printTabs(int tabCount)
{
    for(int i = 0; i < tabCount; i++){
        fprintf(_fptr, "\t");
    }
}


static void _dumpNode(Node* node, int blockLevel)
{
    switch (node->nodeType) {
        case STMT_LIST: {
            NodeStmtList* stmtVec = static_cast<NodeStmtList*>(node);

            int stmtVecSize = getStmtListSize(stmtVec);

            for(int index = 0; index < stmtVecSize; index++) {
                NodeStatement* currentNode = indexStmtList(stmtVec, index);
                printTabs(blockLevel);    
                fprintf(_fptr, "%s:{\n", nodeTypeToString(currentNode->nodeType).c_str());



                printTabs(blockLevel);    
                fprintf(_fptr, "}\n");
            }
            return;
        }
        default: {
            std::cout << "HIT DEFAULT CASE: " << node->nodeType << std::endl;
        }
    }
}


/* Entry point to this module */
void programToJson(NodeStmtList* head, const char* fileLocation)
{
    _fptr = fopen(fileLocation, "w+");

    if(!_fptr){
        std::cout << "ERROR OPENING FILE "<< fileLocation << " IN WRITE MODE FILE: " << fileLocation << std::endl;
        return;
    }

    fprintf(_fptr, "stmt_list:{\n"); 

    _dumpNode(head, 1);

    fprintf(_fptr, "}"); 
}

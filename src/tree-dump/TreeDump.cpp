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
        case DECL_LIST: {

            printTabs(blockLevel);
            fprintf(_fptr, "DECL_LIST_NODE: {\n");

            if(node){
                NodeDeclList* declListNode = static_cast<NodeDeclList*>(node);
                int size = getDeclListSize(declListNode);
                for(int index = 0; index < size; index++){
                    _dumpNode(indexDeclList(declListNode, index), blockLevel + 1);
                }
            }

            printTabs(blockLevel);
            fprintf(_fptr, "},\n");

            return;
        }
        case STMT_LIST: {
            printTabs(blockLevel);    
            fprintf(_fptr, "STMT_LIST_NODE: {\n");

            if(node){
                NodeStmtList* stmtVec = static_cast<NodeStmtList*>(node);
                int stmtVecSize = getStmtListSize(stmtVec);
                for(int index = 0; index < stmtVecSize; index++) {
                    _dumpNode(indexStmtList(stmtVec, index), blockLevel + 1); 
                }
            }

            printTabs(blockLevel);    
            fprintf(_fptr, "},\n");

            return;
        }
        case BLOCK: {
            NodeBlock* block = static_cast<NodeBlock*>(node);
            printTabs(blockLevel);
            fprintf(_fptr, "BLOCK_NODE: {\n");

            if(block->stms){
                _dumpNode(block->stms, blockLevel + 1);
            }

            printTabs(blockLevel);
            fprintf(_fptr, "},\n");

            return;
        }
        case FUNCTION: { 
            NodeFunction* currFunc = static_cast<NodeFunction*>(node);
            printTabs(blockLevel);
            fprintf(_fptr, "FUNCTION_NODE: {\n");

            if(node){
                _dumpNode(currFunc->id, blockLevel + 1);
                _dumpNode(currFunc->arguments, blockLevel + 1);
                _dumpNode(currFunc->returnType, blockLevel + 1);
                _dumpNode(currFunc->block, blockLevel + 1);
            }

            printTabs(blockLevel);
            fprintf(_fptr, "},\n");
            return;
        }
        case ID: {
            std::cout << blockLevel << std::endl;
            printTabs(blockLevel);
            fprintf(_fptr, "ID_NODE: {\n");

            printTabs(blockLevel + 1);
            if(node){
                NodeIdentifier* idNode = static_cast<NodeIdentifier*>(node);
                fprintf(_fptr, "name: %s,\n", idNode->idName);
            }

            printTabs(blockLevel);
            fprintf(_fptr, "},\n");
            return;
        }
        case DECL: {
            printTabs(blockLevel);
            fprintf(_fptr, "DECL_NODE: {\n");

            if(node){
                NodeDecl* declNode = static_cast<NodeDecl*>(node);
                _dumpNode(declNode->id, blockLevel + 1);
                _dumpNode(declNode->type, blockLevel + 1);
            }

            printTabs(blockLevel);
            fprintf(_fptr, "},\n");

            return;
        }
        case TYPE: {
            NodeType* typeNode = static_cast<NodeType*>(node);

            printTabs(blockLevel);
            fprintf(_fptr, "TYPE_NODE: {\n");

            printTabs(blockLevel + 1);
            fprintf(_fptr, "type : %s,\n", idTypeToString(typeNode->idType).c_str());

            printTabs(blockLevel);
            fprintf(_fptr, "},\n");
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


    std::cout << "STARTING DUMPING NODE" << std::endl;
    _dumpNode(head, 0);
    std::cout << "ENDING DUMPING NODE" << std::endl;

}

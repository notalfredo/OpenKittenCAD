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
            std::cout << "ONE" << std::endl;

            printTabs(blockLevel += 1);
            fprintf(_fptr, "DECL_LIST_NODE: {\n");

            if(node){
                NodeDeclList* declListNode = static_cast<NodeDeclList*>(node);
                int size = getDeclListSize(declListNode);
                for(int index = 0; index < size; index++){
                    _dumpNode(indexDeclList(declListNode, index), blockLevel += 1);
                }
            }

            fprintf(_fptr, "}\n");
            printTabs(blockLevel += 1);

            std::cout << "RETURNING" << std::endl;
            return;
        }
        case STMT_LIST: {
            std::cout << "TWO" << std::endl;



            printTabs(blockLevel);    
            fprintf(_fptr, "STMT_LIST_NODE :{\n");

            if(node){
                NodeStmtList* stmtVec = static_cast<NodeStmtList*>(node);
                int stmtVecSize = getStmtListSize(stmtVec);
                for(int index = 0; index < stmtVecSize; index++) {
                    _dumpNode(indexStmtList(stmtVec, index), blockLevel += 1); 
                }
            }

            printTabs(blockLevel);    
            fprintf(_fptr, "}\n");

            std::cout << "RETURNING" << std::endl;
            return;
        }
        case BLOCK: {
            std::cout << "THREE" << std::endl;

            NodeBlock* block = static_cast<NodeBlock*>(node);
            printTabs(blockLevel);
            fprintf(_fptr, "BLOCK_NODE: {\n");

            if(block){
                _dumpNode(block->stms, blockLevel += 1);
            }

            printTabs(blockLevel);
            fprintf(_fptr, "}\n");

            std::cout << "RETURNING" << std::endl;
            return;
        }
        case FUNCTION: { 
            std::cout << "FOUR" << std::endl;

            NodeFunction* currFunc = static_cast<NodeFunction*>(node);
            printTabs(blockLevel);
            fprintf(_fptr, "FUNCTION_NODE: {\n");

            if(node){
                std::cout << "FUNCITO ARG " << nodeTypeToString(currFunc->arguments->nodeType) << std::endl;

                _dumpNode(currFunc->id, blockLevel += 1);
                _dumpNode(currFunc->arguments, blockLevel += 1);
                _dumpNode(currFunc->returnType, blockLevel += 1);
                _dumpNode(currFunc->block, blockLevel += 1);
            }

            printTabs(blockLevel);
            fprintf(_fptr, "}\n");
            std::cout << "RETURNING" << std::endl;
            return;
        }
        case ID: {
            std::cout << "FIVE" << std::endl;

            
            printTabs(blockLevel);
            fprintf(_fptr, "ID_NODE: {\n");

            printTabs(blockLevel += 1);

            if(node){
                NodeIdentifier* idNode = static_cast<NodeIdentifier*>(node);
                fprintf(_fptr, "name: %s", idNode->idName);
            }

            fprintf(_fptr, "}\n");

            std::cout << "RETURNING" << std::endl;
            printTabs(blockLevel);
        }
        case DECL: {
            std::cout << "SIX" << std::endl;

            printTabs(blockLevel);
            fprintf(_fptr, "DECL_NODE: {\n");

            if(node){
                NodeDecl* declNode = static_cast<NodeDecl*>(node);

                std::cout << "HERE" << std::endl;
                if(declNode->id){

                    NodeIdentifier* temp = declNode->id;
                    std::cout << "CHECKING" << std::endl;
                    std::cout << "NODE TYPE: " << temp->nodeType << std::endl;


                    if(!temp->idName){
                        std::cout << "ID NAME IS NULL" << std::endl;
                    }

                    std::cout << "not NULL" << std::endl;
                }
                if(declNode->type){
                    std::cout << "NOT NULL" << std::endl;
                }

                std::cout << "HERE 2" << std::endl;

                _dumpNode(declNode->id, blockLevel += 1);
                std::cout << "HERE 3" << std::endl;

                _dumpNode(declNode->type, blockLevel += 1);
                std::cout << "HERE 4" << std::endl;
            }

            fprintf(_fptr, "}\n");
            printTabs(blockLevel);

            std::cout << "RETURNING" << std::endl;
            return;
        }
        case TYPE: {
            std::cout << "SEVEN" << std::endl;

            NodeType* typeNode = static_cast<NodeType*>(node);

            printTabs(blockLevel);
            fprintf(_fptr, "TYPE_NODE: {\n");

            printTabs(blockLevel += 1);
            fprintf(_fptr, "type : %s", idTypeToString(typeNode->idType).c_str());
            printTabs(blockLevel);

            std::cout << "RETURNING" << std::endl;
            fprintf(_fptr, "}\n");
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

    std::cout << "STARTING DUMPING NODE" << std::endl;
    _dumpNode(head, 1);
    std::cout << "ENDING DUMPING NODE" << std::endl;

    fprintf(_fptr, "}"); 
}

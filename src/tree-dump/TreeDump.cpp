#include "node.hxx"
#include "TreeDumpHelper.hxx"

#include <bits/fs_fwd.h>
#include <stdio.h>
#include <vector>
#include <iostream>


/*Private variables and functions to this mmodule*/
static FILE* _fptr = NULL;
static int PRINT_COMMA = 0;

void printTabs(int tabCount)
{
    for(int i = 0; i < tabCount; i++){
        fprintf(_fptr, "\t");
    }
}

void printText(int tabCount, const char* text)
{
    for(int i = 0; i < tabCount; i++){
        fprintf(_fptr, "\t");
    }
    fprintf(_fptr, "%s", text);
}


static void _dumpNode(Node* node, int blockLevel)
{
    switch (node->nodeType) {
        case DECL_LIST: {
            printText(blockLevel, "\"DECL_LIST_NODE\": [\n");

            if(node){
                NodeDeclList* declListNode = static_cast<NodeDeclList*>(node);
                int size = getDeclListSize(declListNode);
                for(int index = 0; index < size; index++){
                    printText(blockLevel + 1, "{\n");
                    _dumpNode(indexDeclList(declListNode, index), blockLevel + 2);

                    if(index == size - 1){
                        printText(blockLevel + 1, "}\n");
                    }
                    else{
                        printText(blockLevel + 1, "},\n");
                    }
                }
            }

            //PRINT_COMMA ? printText(blockLevel, "],\n") : printText(blockLevel, "]\n");
            printText(blockLevel, "],\n");
            return;
        }
        case STMT_LIST: {
            printText(blockLevel, "\"STMT_LIST_NODE\": [\n");
            if(node){
                NodeStmtList* stmtVec = static_cast<NodeStmtList*>(node);
                int stmtVecSize = getStmtListSize(stmtVec);
                for(int index = 0; index < stmtVecSize; index++) {
                    printText(blockLevel + 1, "{\n");
                    _dumpNode(indexStmtList(stmtVec, index), blockLevel + 2); 

                    if(index == stmtVecSize - 1){
                        printText(blockLevel + 1, "}\n");
                    }
                    else{
                        printText(blockLevel + 1, "},\n");
                    }
                }
            }
            printText(blockLevel, "]\n");
            return;

        }
        case BLOCK: {
            NodeBlock* block = static_cast<NodeBlock*>(node);

            printText(blockLevel, "\"BLOCK_NODE\": {\n"); 

            if(block->stms){
                _dumpNode(block->stms, blockLevel + 1);
            }

            printText(blockLevel, "}\n");
            return;
        }
        case FUNCTION: { 
            NodeFunction* currFunc = static_cast<NodeFunction*>(node);
            printText(blockLevel,  "\"FUNCTION_NODE\": {\n");

            if(node){
                _dumpNode(currFunc->id, blockLevel + 1);
                _dumpNode(currFunc->arguments, blockLevel + 1);
                _dumpNode(currFunc->returnType, blockLevel + 1);
                _dumpNode(currFunc->block, blockLevel + 1);
            }

            printText(blockLevel, "}\n");
            return;
        }
        case ID: {
            printText(blockLevel,  "\"ID_NODE\": {\n");

            if(node){
                NodeIdentifier* idNode = static_cast<NodeIdentifier*>(node);
                std::string text = "\"name\": \"" + std::string(idNode->idName) + "\"\n";

                printText(blockLevel + 1, text.c_str());
            }

            printText(blockLevel, "},\n");
            return;
        }
        case DECL: {
            printText(blockLevel, "\"DECL_NODE\": {\n");

            if(node){
                NodeDecl* declNode = static_cast<NodeDecl*>(node);
                _dumpNode(declNode->id, blockLevel + 1);
                _dumpNode(declNode->type, blockLevel + 1);
                
                printText(blockLevel + 1, "\"value\": {\n");
                if(declNode->value){
                    _dumpNode(declNode->value, blockLevel + 2);
                }
                printText(blockLevel + 1, "}\n");
            }

            printText(blockLevel, "}\n");
            return;
        }
        case TYPE: {
            NodeType* typeNode = static_cast<NodeType*>(node);

            printText(blockLevel, "\"TYPE_NODE\": {\n");

            std::string temp = "\"type\" : \"" + idTypeToString(typeNode->idType) + "\"\n";
            printText(blockLevel + 1, temp.c_str() );

            printText(blockLevel, "},\n");
            return;
        }
        case BIN_OP: {
            if(node){
                NodeBinaryOperator* binOpNode = static_cast<NodeBinaryOperator*>(node);

                printText(blockLevel,  "\"BIN_OP_NODE\": {\n");
                std::string text = "\"type\": \"" + stringFromNodeOp(binOpNode->binaryOperatorType) + "\",\n";
                printText(blockLevel + 1, text.c_str());


                printText(blockLevel + 1, "\"lhs\": {\n");
                _dumpNode(binOpNode->lhs, blockLevel + 2);
                printText(blockLevel + 1, "},\n"); 

                printText(blockLevel + 1, "\"rhs\": {\n");
                _dumpNode(binOpNode->rhs, blockLevel + 2);
                printText(blockLevel + 1, "}\n"); 

                printText(blockLevel, "}\n"); 
            }
            break;
        }
        case DOUBLE: {
            if(node){
                NodeNumber* numberNode = static_cast<NodeNumber*>(node); 

                printText(blockLevel, "\"DOUBLE_NODE\": {\n");

                std::string text = "\"number\": " + std::to_string(numberNode->value) + "\n";
                printText(blockLevel + 1, text.c_str());

                printText(blockLevel, "}\n");
            }
            return;
        }
        default: {
            std::cout << "HIT DEFAULT CASE: " << nodeTypeToString( node->nodeType) << std::endl;
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

    printText(0, "{\n");
    _dumpNode(head, 1);
    printText(0, "}\n");

    std::cout << "ENDING DUMPING NODE" << std::endl;

}

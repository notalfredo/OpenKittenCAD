#include "semantic.hxx"
#include "node.hxx"
#include "stdio.h"
#include "string.h"

static int blockNumber = 0;


static struct functionArg* _argFromNodeDecl(NodeDecl* declNode)
{
    FUNCTION_ARG * curr = (FUNCTION_ARG*)calloc(1, sizeof(FUNCTION_ARG));
    curr->name = strdup(declNode->id->idName);
    curr->argType = declNode->type->idType;
    curr->next = NULL;

    return curr;
}


static FUNCTION_ARG* _functionArgsFromDeclList(NodeDeclList* list)
{
    NodeDecl* curr = list->nextDecl;
     
    if (!curr) {
        return NULL;
    }
    
    FUNCTION_ARG* temp = _argFromNodeDecl(curr);
      
    while(curr->nextDecl){
        FUNCTION_ARG* newArg = _argFromNodeDecl(curr->nextDecl);
        newArg->next = temp;
        temp = newArg;
    }

    return temp;
}


int getCurrentSize(SymbolTableHead* symTable)
{
    int count = 1;
    struct BasicBlock* curr = symTable;

    while (curr->next) {
        curr = curr->next;
        count += 1; 
    }

    return count;
}


SymbolTableHead* newSymbolTable()
{
    SymbolTableHead* symTable = (SymbolTableHead*)calloc(1, sizeof(SymbolTableHead));
    symTable->next = NULL;
    symTable->sym = NULL;
    symTable->blockNumber = 0;
    blockNumber += 1;

    return symTable;
}


void appendNewBasicBlock(SymbolTableHead** symTable)
{
    struct BasicBlock* tempHead = *symTable;
    struct BasicBlock* newBlock = (BasicBlock*)calloc(1, sizeof(BasicBlock));

    newBlock->next = tempHead;
    newBlock->sym = NULL;
    newBlock->blockNumber = blockNumber;

    blockNumber += 1;

    (*symTable) = newBlock;
}


int findSymbol(SymbolTableHead* symTable, const char* name)
{
    for(struct BasicBlock* currBlock = symTable; currBlock != NULL; currBlock = currBlock->next){
        for(Symbol* currSym = currBlock->sym; currSym != NULL; currSym = currSym->next){
            if(!strcmp(name, currSym->name)){
                return 1; 
            }
       }
    }
    return 0;
}

/*
 * From a given NodeDecl* or a NodeFunction*
 * we create a symbol Symbol* for our symbol table
*/
static Symbol* _symbolFromTreeNode(Node* node)
{
    switch (node->nodeType) {
        case DECL: {
            NodeDecl* declNode = static_cast<NodeDecl*>(node);
            Symbol* newSym = (Symbol*)calloc(1, sizeof(Symbol));
            newSym->name = strdup(declNode->id->idName);
            newSym->symbolType = variable;
            newSym->next = NULL;
            
            switch (declNode->type->idType){
                case num: {
                    NodeNumber* numNode = static_cast<NodeNumber*>(declNode->value);
                    newSym->idType = num;
                    newSym->numVal = numNode->value;
                    break;
                }
                case shape: {
                    NodeShape* shapeNode = static_cast<NodeShape*>(declNode->value);
                    newSym->idType = shape;
                    newSym->shapeType = shapeNode->shape;
                    break;
                }
                case _void: {
                    fprintf(stderr, "HIT _VOID IN SWITCH INSIDE symbolFromTreeNode\n");
                    return NULL;
                }
            }
            return newSym;
        }
        case FUNCTION: {
            NodeFunction* functionNode = static_cast<NodeFunction*>(node);
            Symbol* newSym = (Symbol*)calloc(1, sizeof(Symbol));
            newSym->name = strdup(functionNode->id->idName);
            newSym->symbolType = function;
            newSym->returnType = functionNode->returnType->idType;
            newSym->functionArgs = _functionArgsFromDeclList(functionNode->arguments);
            return newSym;
        }
        default: {
            fprintf(stderr, "HIT THE DEFAULT CASE symbolFromTreeNode()\n");
            return NULL;
        }
    }
}


//Insters symbol at the top most basic block
void insertSymbolFromNode(SymbolTableHead* symTable, Node* node)
{
    Symbol* temp = symTable->sym;

    //Our head might be null, check if our head is not null
    if(temp){
        symTable->sym = _symbolFromTreeNode(node);
        symTable->sym->next = temp;
        return;
    }
    symTable->sym = _symbolFromTreeNode(node);
}

void freeSymbolList(Symbol** symbolList)
{
    //If our symbol list is empty return
    if(!(*symbolList)){
        return;
    }

    Symbol* curr = *symbolList;
    Symbol* temp = NULL;
    
    while (curr) {
        temp = curr->next; 
        free(curr); 
        curr = temp;
    }

    *symbolList = NULL;
}


void freeTopBlock(SymbolTableHead** symTable, int dump)
{
    //We might be the last block
    if(!(*symTable)->next){
        fprintf(stderr, "FREEING TOP BLOCK\n");
        freeSymbolList(&(*symTable)->sym);
        free(*symTable);
        (*symTable) = NULL;
    }
    else{
        fprintf(stderr, "FREEING NON TOP BLOCK\n");
        SymbolTableHead* currHead = (*symTable);
        SymbolTableHead* newHead = (*symTable)->next;

        freeSymbolList(&currHead->sym);
        free(currHead);
        *symTable = newHead;
    }
}


void dumpSymbolLinkedList(Symbol* head)
{    
    if(!head){
        fprintf(stderr, "\tsymLinkedListIsEmpty\n");
    }

    Symbol* temp = head;    

    while (temp) {
        switch (temp->symbolType) {
            case variable: {
                fprintf(stderr, "\tSYMBOL (");
                fprintf(stderr, "\tname: %s", temp->name);
                fprintf(stderr, "\tidType: %d", temp->idType);

                switch (temp->idType) {
                    case num: {
                        fprintf(stderr, "\tnumVal: %f", temp->numVal);
                        break;
                    }
                    case shape: {
                        fprintf(stderr, "\tshapeVal: %d", temp->shapeType);
                        break;
                    }
                    case _void: {
                        fprintf(stderr, "INSIDE dumpSymbolTable()\n");
                        break;
                    }
                }
                fprintf(stderr, ")\n");
                break;
            }
            case function: {
                fprintf(stderr, "TODO\n");
                break;
            }
        }
        temp = temp->next;
    }
}

void dumpSymbolTable(SymbolTableHead* head)
{
    SymbolTableHead* temp = head; 

    while(temp){
        fprintf(stderr, "Block num %d (\n", temp->blockNumber);
        dumpSymbolLinkedList(temp->sym);
        fprintf(stderr, ")\n");
        temp = temp->next;
    }
}





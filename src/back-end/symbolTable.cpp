#include "symbolTable.hxx"
#include "node.hxx"
#include "stdio.h"
#include "string.h"

static int blockNumber = 1;



/* 
   =================================================== 
   ================= FREEING MEMMORY ================= 
   =================================================== 
*/
void freeSymbolNode(Symbol** symbolNode)
{
    free((*symbolNode)->name);
    if((*symbolNode)->functionArgs){
        //TODO FREE FUNCTION ARGS
    }
    free(*symbolNode);
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
        freeSymbolNode(&curr); 
        curr = temp;
    }

    *symbolList = NULL;
}


void freeTopBlock(SymbolTableHead** symTable)
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


void freeSymbolTable(SymbolTableHead** symTableHead)
{
    while(*symTableHead){
        freeTopBlock(symTableHead);
    }
}
/* 
   =================================================== 
   =================================================== 
   =================================================== 
*/


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



SymbolTableHead* newSymbolTable()
{
    SymbolTableHead* symTable = (SymbolTableHead*)calloc(1, sizeof(SymbolTableHead));
    symTable->next = NULL;
    symTable->sym = NULL;
    symTable->blockNumber = blockNumber;
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
            newSym->functionArgs = NULL;
            
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


//Insterts symbol at the top most basic block
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


int _searchSymbolListForName(Symbol* symList, const char* searchName)
{
    Symbol* curr = symList; 

    while(curr){
        if(!strcmp(curr->name, searchName)){
            return 1; 
        }
        curr = curr->next;
    }

    return 0;
}

/*
 * Returns block number if found symbol with matching name.
 * Else returns 0
*/
int containsSymbolName(SymbolTableHead* symTable, const char* searchName)
{
    SymbolTableHead* curr = symTable;

    while(curr){
        if(_searchSymbolListForName(curr->sym, searchName)){
            return curr->blockNumber;
        }
        curr = curr->next;
    }

    return 0;
}

int getCurrentSize(SymbolTableHead* symTable)
{
    if(!symTable){
        return 0;
    }

    struct BasicBlock* curr = symTable;
    int count = 1;

    while (curr->next) {
        curr = curr->next;
        count += 1; 
    }

    return count;
}


// WARNING ONLY USE THIS DURING TESTING
void resetBlockCounter()
{
    blockNumber = 1;
}

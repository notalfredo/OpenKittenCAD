#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "node.hxx"
#include <vector>

/*
          NULL 
           ^ 
           | 
           |       
    ---------------       ----------------                 ----------------           
    |             |  sym  |              |                 |              |           
    |    BLOCK    | ----> |    Symbol    | ----> ... ----> |    Symbol    | ----> NULL
    |             |       |              |                 |              |           
    ---------------       ----------------                 ----------------           
           ^ 
           | 
           |                                 

          ...

           ^ 
           | 
           |                                 
    ---------------       ----------------                 ----------------           
    |             |  sym  |              |                 |              |           
    |    BLOCK    | ----> |    Symbol    | ----> ... ----> |    Symbol    | ----> NULL
    |             |       |              |                 |              |           
    ---------------       ----------------                 ----------------           
           ^
           | next
           |
    ---------------       ----------------                 ----------------
    |             |  sym  |              |                 |              |
    |    BLOCK    | ----> |    Symbol    | ----> ... ----> |    Symbol    | ---> NULL
    |    NULL     |       |              |                 |              |
    |             |       ----------------                 ----------------    
    --------------- 
*/

typedef enum symType {
    variable,
    function, 
} SYMBLE_TYPE;


typedef struct functionArg {
    char* name;
    ID_TYPE argType; 
    struct functionArg* next;
} FUNCTION_ARG;


typedef struct symbol {
    char* name;
    
    SYMBLE_TYPE symbolType; // If we have a variable or function
                            //
    NodeFunction* function;
                        
    ID_TYPE  idType; // If we are a variable are we a number, shape ... MAKE SURE NOT TO ASSIGN VOID IF VARIABLE
                     
    NodeNumber* numVal; //If variable and a number assign here

    OCCT_SHAPE shapeType; //If we are a shape assign our shape type here
    NodeShape* shape;

    NodePoint* point; //IF we are a point assign our point here
                      
    NodeLine* edge; 

    struct symbol* next;
} Symbol ;


typedef struct BasicBlock {
    struct BasicBlock* next; 
    Symbol* sym;
    int blockNumber;
} SymbolTableHead;


SymbolTableHead* newSymbolTable();

void freeTopBlock(SymbolTableHead** symTable);
void freeSymbolTable(SymbolTableHead** symTableHead);

void appendNewBasicBlock(SymbolTableHead** symTable);
void insertSymbolFromNode(SymbolTableHead* symTable, Node* node);

int containsSymbolName(SymbolTableHead* symTable, const char* searchName);
Symbol* getSymbolNode(SymbolTableHead* symTable, const char* searchName);
int getCurrentSize(SymbolTableHead* symTable);
void resetBlockCounter();


SymbolTableHead* functionCallNewSymbolTable(SymbolTableHead* currentSymbolTable, Symbol* sym);
void freeFunctionCallSymbolTable(SymbolTableHead** currentSymbolTable, Symbol* sym);


void dumpSymbolLinkedList(Symbol* head);
void dumpSymbolTable(SymbolTableHead* head);


#endif

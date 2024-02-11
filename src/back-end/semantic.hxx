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
    ID_TYPE returnType; // If we are a function what do we return 
    FUNCTION_ARG* functionArgs;
                        
    ID_TYPE  idType; // If we are a variable are we a number, shape ... MAKE SURE NOT TO ASSIGN VOID IF VARIABLE
    double numVal; //If variable and a number assign here
    OCCT_SHAPE shapeType; //If we are a shape assign our shape type here
                          
    struct symbol* next;
} Symbol ;


typedef struct BasicBlock {
    struct BasicBlock* next; 
    Symbol* sym;
    int blockNumber;
} SymbolTableHead;



SymbolTableHead* newSymbolTable();
void appendBasicBlock(SymbolTableHead** symTable);
int findSymbol(SymbolTableHead* symTable, const char* name);

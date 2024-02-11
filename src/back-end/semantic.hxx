#include "node.hxx"

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
    ---------------
    |             |
    |    BLOCK    |
    |             |
    ---------------
           ^
           | next
           |
    ---------------       ----------------------
    |             |  head |                    |
    |    BLOCK    | <---- |    Symbol Table    |
    |             |       |                    |    
    ---------------       ----------------------
*/

//types of valid symbols
typedef enum symType {
    variable,
    function, 
} SYMBLE_TYPE;



typedef struct symbol {
    char* name;
    
    SYMBLE_TYPE symbolType; // If we have a variable or function
    ID_TYPE returnType; // If we are a function what do we return 
    //SOME SORT OF FUNCTION POINTER
                        
                        
    ID_TYPE  idType; // If we are a variable are we a number, shape ... MAKE SURE NOT TO ASSIGN VOID IF VARIABLE
    double numVal; //If variable and a number assign here
    //TODO shape shapeVal
    


} Symbol ;


typedef struct BasicBlock {
    struct BasicBlock* next; 
    Symbol sym;
} SymbolTableHead;

SymbolTableHead* newSymbolTable();
void appendBasicBlock(SymbolTableHead* symTable);

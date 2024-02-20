#include "node.hxx"
#include "functions.hxx"
#include <cstdio>
#include <cstring>
#include <system_error>


void _print(double num)
{
    fprintf(stdout, "*********************%f\n", num);
}

functionPtr knownFunctions[] {
    {"print", printDouble, _print}
};
const int SIZE_OF_FUNS = sizeof(knownFunctions) / sizeof (functionPtr);

functionPtr* lookUpFunc(const char * funcName)
{
    fprintf(stderr, "Looking up %s\n", funcName);
    for(int index = 0; index < SIZE_OF_FUNS; index++){
        if(!strcmp(knownFunctions[index].name, funcName)){
            return &knownFunctions[index];
        }
    }
    return NULL;
}


//TODO: WHEN WE CALL A FUNCTION RIGHT BEFORE WE CALL IT WE NEED 
//      TO VERIFY THAT THE CORRECT ARGUMENTS WHERE PASSED IN
void execFunc(functionPtr* functionPtr, NodeExpression* paramInfo)
{
    switch(functionPtr->functionType){
        case printDouble: {
            fprintf(stderr, "printDouble\n");
            NodeNumber* numNode = static_cast<NodeNumber*>(paramInfo);
            functionPtr->func.println(numNode->value);
            break;
        }
    }
}

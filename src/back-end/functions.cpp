#include "node.hxx"
#include "functions.hxx"
#include <cstdio>
#include <cstring>
#include <system_error>


void _print(double num)
{
    fprintf(stdout, "%f\n", num);
}

function knownFunctions[] {
    {"print", printDouble, _print}
};
const int SIZE_OF_FUNS = sizeof(knownFunctions) / sizeof (function);

function* lookUpFunc(const char * funName)
{
    for(int index = 0; index < SIZE_OF_FUNS; index++){
        if(!strcmp(knownFunctions[index].name, funName)){
            return &knownFunctions[index];
        }
    }
    return NULL;
}


void execFunc(function* functionPtr, NodeExpression* paramInfo)
{
    switch(functionPtr->functionType){
        case printDouble: {
            NodeNumber* numNode = static_cast<NodeNumber*>(paramInfo);
            functionPtr->func.println(numNode->value);
            break;
        }
    }
}

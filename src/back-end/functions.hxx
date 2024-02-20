#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"

enum functionEnum {
    printDouble,
};

union functionPointers {
    void (*println)(double); 
};


typedef struct function{
    const char* name;
    functionEnum functionType;
    functionPointers func;
}function;


void _print(double num);
function* lookUpFunc(const char*);
void execFunc(function* functionPtr, NodeExpression* paramInfo);

#endif

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"


enum functionEnum {
    printDouble,
};


union functionPointers {
    void (*println)(double); 
};


typedef struct functionPtr{
    const char* name;
    functionEnum functionType;
    functionPointers func;
}functionPtr;


void _print(double num);
functionPtr* lookUpFunc(const char*);
void execFunc(functionPtr* functionPtr, NodeExpression* paramInfo);


#endif

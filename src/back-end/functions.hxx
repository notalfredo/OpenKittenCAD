#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>



enum functionEnum {
    printDouble,
    makeSphere    
};


union functionPointers {
    NodeShape* (*makeSphere) (double);
    void (*println)(double); 
};


typedef struct functionPtr{
    const char* name;
    functionEnum functionType;
    functionPointers func;
}functionPtr;


void _print(double num);
functionPtr* lookUpFunc(const char*);
Node* execFunc(functionPtr* functionPtr, NodeExpression* paramInfo);


#endif

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>



enum functionEnum {
    printDouble,
    makeSphere,
    addSphere
};


union functionPointers {
    NodeShape* (*makeSphere) (double);
    void (*println) (double); 
    void (*addShapeToVTK) (const TopoDS_Shape&);
};


typedef struct functionPtr{
    const char* name;
    functionEnum functionType;
    functionPointers func;
}functionPtr;


void initViewer();
void startViewer();
void _addShape(const TopoDS_Shape& shapeToAdd);

void _print(double num);
functionPtr* lookUpFunc(const char*);
NodeExpression* execFunc(functionPtr* functionPtr, NodeExpression* paramInfo);


#endif

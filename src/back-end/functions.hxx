#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"


enum functionEnum {
    makeSphere,
    makeCone,
    makeCylinder,
    makeBox,

    printDouble,
    addShape
};


union functionPointers {
    NodeShape* (*makeSphere) (double);
    NodeShape* (*makeBox) (double, double, double);
    NodeShape* (*makeCone) (double, double, double);
    NodeShape* (*makeCylinder) (double, double);

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
NodeExpression* execFunc(functionPtr* functionPtr, std::vector<NodeExpression*>& args);


#endif

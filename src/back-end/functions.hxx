#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"


enum functionEnum {
    makeSphere,
    makeCone,
    makeCylinder,
    makeBox,
    makeUnion,
    makeDifference,
    makeIntersection,

    
    doRotate,
    doTranslate,

    makePoint,
    

    printDouble,
    addShape
};


union functionPointers {
    NodeShape* (*makeSphere) (double);
    NodeShape* (*makeBox) (double, double, double);
    NodeShape* (*makeCone) (double, double, double);
    NodeShape* (*makeCylinder) (double, double);
    NodeShape* (*makeUnion) (const TopoDS_Shape&, const TopoDS_Shape&);
    NodeShape* (*makeDifference) (const TopoDS_Shape&, const TopoDS_Shape&);
    NodeShape* (*makeIntersection) (const TopoDS_Shape&, const TopoDS_Shape&);


    NodeShape* (*rotate) (const TopoDS_Shape&, double,  OCCT_SHAPE, gp_Ax1 xAxis);
    NodeShape* (*translate) (const TopoDS_Shape&, double, double, double, OCCT_SHAPE);

    
    NodePoint* (*makePoint) (double, double, double);


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
void resetViewer();
void _addShape(const TopoDS_Shape& shapeToAdd);

void _print(double num);
functionPtr* lookUpFunc(const char*);
NodeExpression* execFunc(functionPtr* functionPtr, std::vector<NodeExpression*>& args);


#endif

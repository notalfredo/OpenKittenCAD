#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"


typedef struct paramInfo {
    nodeType type;
    std::string name;
} PARAM_INFO;


enum functionEnum {
    makeSphere,
    makeCone,
    makeCylinder,
    makeBox,
    makeUnion,
    makeDifference,
    makeIntersection,
    makeFace,

    
    doRotate,
    doTranslate,
    doMirror,

    makePoint,
    makeEdge,
    makeArc,
    connect,
    

    printDouble,
    addShape
};



typedef struct functionPtr{
    const char* name;
    functionEnum functionType;
}functionPtr;


void initViewer();
void startViewer();
void resetViewer();
void _addShape(const TopoDS_Shape& shapeToAdd);

functionPtr* lookUpFunc(const char*);
NodeExpression* execFunc(functionPtr* functionPtr, std::vector<NodeExpression*>& args);


#endif

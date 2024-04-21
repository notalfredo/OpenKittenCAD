#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "node.hxx"


enum twoDim {
    makePoint,
    makeEdge,
    makeArc,
    makeFace,
};


enum threeDim {
    makeSphere,
    makeCone,
    makeCylinder,
    makeBox,
    makeTorus,
};


enum booleanOperations {
    performUnion,
    performDifference,
    performIntersection,
};


enum transformation {
    performRotate,
    performTranslate,
    performMirror,
};


enum geomOp {
    performExtrude,
    performFillet,
    performChamfer,
    performRevolve,
};


enum miscFunc {
    print,
    addShape,
    createConnection,
    createLineTo,
};


union functionCategory {
    twoDim twoDimCall;
    threeDim threeDimCall;
    booleanOperations booleanOperationsCall;
    transformation transformationCall;
    geomOp geomOpCall;
    miscFunc miscFuncCall;
};


enum functionType {
    twoDim,
    threeDim,
    booleanOperations,
    transformation,
    miscFunc,
    geomOp,
};



typedef struct functionPtr{
    const char* name;
    functionType funcType;
    functionCategory functionVal;
}functionPtr;


void initViewer();
void startViewer();
void resetViewer();
void _addShape(const TopoDS_Shape& shapeToAdd);

functionPtr* lookUpFunc(const char*);
NodeExpression* execFunc(functionPtr* functionPtr, std::vector<NodeExpression*>& args);


#endif

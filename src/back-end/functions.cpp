#include "TopoDS_Shape.hxx"
#include "node.hxx"
#include "functions.hxx"

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>

void _print(double num)
{
    fprintf(stdout, "%f\n", num);
}

NodeShape* _makeSphere(Standard_Real r)
{
    BRepPrimAPI_MakeSphere* sphere = new BRepPrimAPI_MakeSphere(2);
    const TopoDS_Shape* shape = &sphere->Shape();

    NodeShape* me = newNodeShape(SPHERE);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(sphere);
    me->shape = shape;

    return me;
}

functionPtr knownFunctions[] {
    {"print",  printDouble, {.println =  _print}},
    {"sphere", makeSphere,  {.makeSphere = _makeSphere}}
};

const int SIZE_OF_FUNS = sizeof(knownFunctions) / sizeof (functionPtr);

functionPtr* lookUpFunc(const char * funcName)
{
    for(int index = 0; index < SIZE_OF_FUNS; index++){
        if(!strcmp(knownFunctions[index].name, funcName)){
            return &knownFunctions[index];
        }
    }
    return NULL;
}


//TODO: WHEN WE CALL A FUNCTION RIGHT BEFORE WE CALL IT WE NEED 
//      TO VERIFY THAT THE CORRECT ARGUMENTS WHERE PASSED IN
Node* execFunc(functionPtr* functionPtr, NodeExpression* paramInfo)
{
    switch(functionPtr->functionType){
        case printDouble: {
            NodeNumber* numNode = static_cast<NodeNumber*>(paramInfo);
            functionPtr->func.println(numNode->value);
            return NULL;
        }
        case makeSphere: {
            /*
             * For now only handaling this function call need to handle other ways to make a sphere in OCCT
             *      BRepPrimAPI_MakeSphere::BRepPrimAPI_MakeSphere( const Standard_Real R ) 	 
            */
            NodeNumber* numNode = static_cast<NodeNumber*>(paramInfo);
            return functionPtr->func.makeSphere(numNode->value);
        }
    }
}

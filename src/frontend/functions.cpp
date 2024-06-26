#include "BRepBuilderAPI_ModifyShape.hxx"
#include "BRepPrimAPI_MakeRevol.hxx"
#include "Geom_TrimmedCurve.hxx"
#include "TopoDS_Wire.hxx"
#include "enumToString.hxx"
#include "gp_Ax1.hxx"
#include "gp_Trsf.hxx"
#include "node.hxx"
#include "functions.hxx"
#include "enumToString.hxx"


#include "Standard_TypeDef.hxx"
#include "TopoDS_Shape.hxx"
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include "BRepBuilderAPI_MakeShape.hxx"
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <TopoDS.hxx>

#include <BRepBuilderAPI_Transform.hxx>

#include <gp_Pnt.hxx>

#include <GC_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include "validateFunc.hxx"

#include <AIS_Shape.hxx>

typedef std::vector<Handle(AIS_Shape)> shapesVector;

static std::vector<NodeShape*> myShapes;


std::vector<NodeShape*> nodeShapeVector()
{
    return myShapes;
}


shapesVector getShapes()
{
    shapesVector aisShapes;

    for(int index = 0; index < myShapes.size(); index++){
        switch(myShapes[index]->nodeType){
            case SHAPE: {
                Handle(AIS_Shape) myShape = new AIS_Shape(
                    static_cast<NodeShape*>(myShapes[index])->brepShape->Shape()
                );

                aisShapes.push_back(myShape);
                break;
            }
            default: {
                fprintf(stderr, "Default case hit in getShapes() ... exiting ... hit %s\n", nodeTypeToString(myShapes[index]->nodeType));
                exit(1);
            }
        }
    }

    return aisShapes;
}


void _addShape(NodeShape* myShape)
{
    myShapes.push_back(myShape);
}


void _print(std::vector<NodeExpression*>& args)
{
    if(args.size() != 1){
        fprintf(stderr, "print only takes 1 argument you passed %ld\n", args.size());
        exit(1);
    }
    else if(!args[0]){
        fprintf(stderr, "print was given a null argument ... exiting ... \n");
        exit(1);
    }

    switch(args[0]->nodeType){
        case DOUBLE:{
            NodeNumber* numNode = static_cast<NodeNumber*>(args[0]);
            fprintf(stdout, "%lf\n", numNode->value);
            return;
        }
        case EDGE: {
            NodeEdge* edgeNode = static_cast<NodeEdge*>(args[0]);        
            switch(edgeNode->edgeType){
                case type_edge: {
                    edgeNode->edge->DumpJson(std::cout);
                    return;
                }
                case type_wire: {
                    edgeNode->wireShape->DumpJson(std::cout);
                    return;
                }
                default: {
                    fprintf(stderr, "Unable to print edge exiting .. \n"); 
                    exit(1);
                }
            }
        }
        case POINT: {
            NodePoint* pointNode = static_cast<NodePoint*>(args[0]);
            fprintf(stderr, "Point {X: %lf, Y: %lf, Z: %lf}\n", pointNode->point->X(), pointNode->point->Y(), pointNode->point->Y());
            return;

        }
        case SHAPE: {
            NodeShape* shapeNode = static_cast<NodeShape*>(args[0]);
            shapeNode->shape->DumpJson(std::cout);
            return;
        }
        default: {
            fprintf(stderr, "You can only print numbers, points, edges, and shapes\n");
            exit(1);
        }
    }
    return;
}


NodeShape* _makeCone(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeCone* cone = _validateCone(args);
    const TopoDS_Shape* shape = &cone->Shape();

    NodeShape* me = newNodeShape(CONE);
    me->brepShape = cone;
    me->shape = shape;

    return me;
}


NodeShape* _makeCylinder(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeCylinder* cyl = _validateCylinder(args);
    const TopoDS_Shape* shape = &cyl->Shape();

    NodeShape* me = newNodeShape(CYLINDER);
    me->brepShape = cyl;
    me->shape = shape;

    return me;
}


NodeShape* _makeBox(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeBox* box = _validateBox(args);
    const TopoDS_Shape* shape = &box->Shape();

    NodeShape* me = newNodeShape(BOX);
    me->brepShape = box;
    me->shape = shape;

    return me;
}


NodeShape* _makeRevolve(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeRevol* revol = _validateRevol(args);
    const TopoDS_Shape* shape = &revol->Shape();

    
    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = revol;
    me->shape = shape;

    return me;
}


NodeExpression* _addShape(std::vector<NodeExpression*>& args)
{
    if(args.size() != 1){
        fprintf(stderr, "You can only pass 1 argument to addShape\n");
            }
    else if(!args[0]){
        fprintf(stderr, "First argument to addShape is NULL\n");
    }
    else if(args[0]->nodeType != SHAPE){
        fprintf(stderr, "Argument to addShape must be type shape you passed %s\n", nodeTypeToString(args[0]->nodeType));
    }


    NodeShape* sphere = static_cast<NodeShape*>(args[0]);
    _addShape(sphere);
    return NULL;
}


NodeShape* _makeUnion(std::vector<NodeExpression*>& args)
{
    BRepAlgoAPI_Fuse* fuse = static_cast<BRepAlgoAPI_Fuse*>(_validateBoolean(args, FUSE));
    const TopoDS_Shape* shape = &fuse->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = fuse;
    me->shape = shape;

    return me;
}

NodeShape* _makeDifference(std::vector<NodeExpression*>& args)
{
    BRepAlgoAPI_Cut* cut = static_cast<BRepAlgoAPI_Cut*>(_validateBoolean(args, DIFFERENCE));
    const TopoDS_Shape* shape = &cut->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = cut;
    me->shape = shape;

    return me;
}


NodeShape* _makeIntersection(std::vector<NodeExpression*>& args)
{
    BRepAlgoAPI_Common* common = static_cast<BRepAlgoAPI_Common*>(_validateBoolean(args, INTERSECTION));
    const TopoDS_Shape* shape = &common->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = common;
    me->shape = shape;

    return me;
}


NodePoint* _makePoint(std::vector<NodeExpression*>& args)
{
    gp_Pnt* point = _validatePoint(args);

    NodePoint* pointNode = newNodePoint();
    pointNode->point = point;

    return pointNode;
}



NodeShape* _makeRotate(std::vector<NodeExpression*>& args)
{
    OCCT_SHAPE shapeType;

    BRepBuilderAPI_Transform* rotations = static_cast<BRepBuilderAPI_Transform*>(_validateRotateTranslate(args, ROTATION, shapeType));

    const TopoDS_Shape* shape = &rotations->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(shapeType);
    me->brepShape = rotations;
    me->shape = shape;

    return me;
}

NodeShape* _makeTranslate(std::vector<NodeExpression*>& args)
{

    OCCT_SHAPE shapeType;

    BRepBuilderAPI_Transform* translation = static_cast<BRepBuilderAPI_Transform*>(_validateRotateTranslate(args, TRANSLATION, shapeType));

    const TopoDS_Shape* shape = &translation->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(shapeType);
    me->brepShape = translation;
    me->shape = shape;
    return me;
}


NodeShape* _makeFace(std::vector<NodeExpression*> args)
{
    BRepBuilderAPI_MakeFace* face = _validateFace(args);


    NodeShape* me = newNodeShape(FACE);
    me->brepShape = face;
    me->shape = &face->Shape();

    return me;
}


NodeArray* _createLineTo(std::vector<NodeExpression*>& args)
{
    return _validateLineTo(args);  
}


NodeEdge* _makeEdge(std::vector<NodeExpression*>& args)
//NodeEdge* _makeEdge(NodePoint* p1, NodePoint* p2)
{
    BRepBuilderAPI_MakeEdge* edge = _validateEdge(args);    
    const TopoDS_Edge* result = &edge->Edge();

    NodeEdge* me = newNodeEdge();
    me->edgeType = type_edge;
    me->brepEdge = edge;
    me->edge = result;

    me->wireShape = NULL;
    me->brepWire = NULL;

    return me;
}


NodeEdge* _makeArc(std::vector<NodeExpression*>& args)
{
    BRepBuilderAPI_MakeEdge* edge = _validateArc(args);    
    const TopoDS_Edge* result = &edge->Edge();

    NodeEdge* me = newNodeEdge();
    me->edgeType = type_edge;
    me->brepEdge = edge;
    me->edge = result;

    me->wireShape = NULL;
    me->brepWire = NULL;

    return me;
}


NodeEdge* _createConnect(std::vector<NodeExpression*> args)
{

    BRepBuilderAPI_MakeWire* brepWire = _validateConnect(args);

    const TopoDS_Wire* wireShape = &brepWire->Wire();


    NodeEdge* me = newNodeEdge();
    me->edge = NULL;
    me->brepEdge = NULL;
    me->edgeType = type_wire;

    me->brepWire = brepWire;
    me->wireShape = wireShape;

    return me;    
}



/*we need to pass some info on what type of shape we are. For now just assume we passed a wire*/
/*Also for now we just assume we can only mirror on the X axis */

NodeExpression* _mirror(std::vector<NodeExpression*>& args) 
{

    _validateMirror(args);

    
    gp_Trsf aTrsf;
    aTrsf.SetMirror(gp::OX());


    switch(args[0]->nodeType){
        case EDGE:{
            NodeEdge* myEdge = static_cast<NodeEdge*>(args[0]);
            TopoDS_Wire wire;


            if(myEdge->edge){
                wire = TopoDS::Wire(BRepBuilderAPI_Transform(*myEdge->edge, aTrsf).Shape());
            }
            else if(myEdge->wireShape){
                wire = TopoDS::Wire(BRepBuilderAPI_Transform(*myEdge->wireShape, aTrsf).Shape());
            }
            else {
                fprintf(stderr, "Unable to mirror shape ... exiting ..\n");
                exit(1);
            }

            NodeEdge* me = newNodeEdge();
            me->edge = NULL;
            me->brepEdge = NULL;
            me->edgeType = type_wire;

            me->brepWire = new BRepBuilderAPI_MakeWire(wire);
            me->wireShape = &me->brepWire->Wire();
            
            return me;
        }
        case SHAPE: {
            NodeShape* myShape = static_cast<NodeShape*>(args[0]);
            BRepBuilderAPI_Transform transform(*myShape->shape, aTrsf);
            
            BRepPrimAPI_MakeSphere* sphere = _validateSphere(args);
            const TopoDS_Shape* shape = &sphere->Shape();

            NodeShape* me = newNodeShape(SPHERE);
            me->brepShape = sphere;
            me->shape = shape;

            return me;
        }
        default: {
            fprintf(stderr, "Hit default case in mirroing shape exiting ...\n");
            exit(1);
        }
    }


}


NodeShape* _makeSphere(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeSphere* sphere = _validateSphere(args);
    const TopoDS_Shape* shape = &sphere->Shape();

    NodeShape* me = newNodeShape(SPHERE);
    me->brepShape = sphere;
    me->shape = shape;

    return me;
}



NodeShape* _makeExtrude(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakePrism* prism = _validateExtrude(args);

    const TopoDS_Shape* shape = &prism->Shape();

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = prism;
    me->shape = shape;

    return me;
}


NodeShape* _makeFillet(std::vector<NodeExpression*>& args)
{
    OCCT_SHAPE shapeType;
    BRepFilletAPI_MakeFillet* myFillet = _validateFillet(args, shapeType);
    const TopoDS_Shape* shape = &myFillet->Shape();

    NodeShape* me = newNodeShape(shapeType);
    me->brepShape = myFillet;
    me->shape = shape;

    return me;
}


NodeShape* _makeChamfer(std::vector<NodeExpression*>& args)
{
    OCCT_SHAPE shapeType;
    BRepFilletAPI_MakeChamfer* myFillet = _validateChamfer(args, shapeType);
    const TopoDS_Shape* shape = &myFillet->Shape();

    NodeShape* me = newNodeShape(shapeType);
    me->brepShape = myFillet;
    me->shape = shape;

    return me;
}


NodeShape* _makeTorus(std::vector<NodeExpression*> &args)
{
    BRepPrimAPI_MakeTorus* myTorus = _validateTorus(args);
    const TopoDS_Shape* shape = &myTorus->Shape();

    NodeShape* me = newNodeShape(TORUS);
    me->brepShape = myTorus;
    me->shape = shape;
    return me;
}


functionPtr knownFunctions[] {
    {"sphere",   threeDim, {.threeDimCall = makeSphere}   },
    {"cone",     threeDim, {.threeDimCall = makeCone}     },
    {"cylinder", threeDim, {.threeDimCall = makeCylinder} },
    {"box",      threeDim, {.threeDimCall = makeBox}      },
    {"torus",    threeDim, {.threeDimCall = makeTorus}    },

    {"union",        booleanOperations, {.booleanOperationsCall = performUnion} },
    {"difference",   booleanOperations, {.booleanOperationsCall = performDifference} },
    {"intersection", booleanOperations, {.booleanOperationsCall = performIntersection} },

    {"rotate",     transformation, {.transformationCall = performRotate}    },
    {"translate",  transformation, {.transformationCall = performTranslate} },
    {"mirror",     transformation, {.transformationCall = performMirror}    },

    {"extrude",    geomOp, {.geomOpCall = performExtrude} },
    {"fillet",     geomOp, {.geomOpCall = performFillet } },
    {"chamfer",    geomOp, {.geomOpCall = performChamfer} },
    {"revolve",    geomOp, {.geomOpCall = performRevolve} },

    {"dot",      twoDim,    {.twoDimCall = makePoint} },
    {"line",     twoDim,    {.twoDimCall = makeEdge} },
    {"arc",      twoDim,    {.twoDimCall = makeArc} },
    {"makeFace", twoDim,    {.twoDimCall = makeFace}},

    {"lineTo",   miscFunc,   {.miscFuncCall = createLineTo}},
    {"connect",  miscFunc,   {.miscFuncCall = createConnection}},
    {"print",    miscFunc,   {.miscFuncCall = print}},
    {"addShape", miscFunc,   {.miscFuncCall = addShape}}
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


NodeExpression* execBooleanOperations(functionCategory category, std::vector<NodeExpression*>& args)
{
    switch(category.booleanOperationsCall){
        case performUnion: {
            return _makeUnion(args);
        }
        case performIntersection: {
            return _makeIntersection(args);
        }
        case performDifference: {
            return _makeDifference(args);
        }
        default: {
            fprintf(stderr, "Hit default case in execBooleanOperations ... exiting ... \n");
            exit(1);
        }
    }
}


NodeExpression* execTwoDim(functionCategory category, std::vector<NodeExpression*>& args)
{
    switch(category.twoDimCall){
        case makePoint: {
            return _makePoint(args);
        }
        case makeEdge: {
            return _makeEdge(args);
        }
        case makeArc: {
            return _makeArc(args);
        }
        case makeFace: {
            return _makeFace(args);
        }
        default: {
            fprintf(stderr, "Hit default case inside of execTwoDim exiting ... ");
            exit(1);
        }
    }
}


NodeExpression* execThreeDim(functionCategory category, std::vector<NodeExpression*>& args)
{
    switch(category.threeDimCall){
        case makeSphere: {
            return _makeSphere(args);
        }
        case makeCone: {
            return _makeCone(args);
        }
        case makeCylinder: {
            return _makeCylinder(args);
        }
        case makeBox: { 
            return _makeBox(args);
        }
        case makeTorus: {
            return _makeTorus(args);
        }
        default: {
            fprintf(stderr, "Hit default case inside of execThreeDim ... exiting ... \n");
            exit(1);
        }
    }
}


NodeExpression* execTransformation(functionCategory category, std::vector<NodeExpression*>& args)
{
    switch(category.transformationCall){
        case performRotate: {
            return _makeRotate(args);
        }
        case performTranslate: { 
            return _makeTranslate(args);
        }
        case performMirror: {
            return _mirror(args);
        }
        default: {
            fprintf(stderr, "Hit default case in execTransformation exiting ... \n");
            exit(1);
        }
    }
}


NodeExpression* execGeomOp(functionCategory category, std::vector<NodeExpression*>& args)
{
    switch (category.geomOpCall) {
        case performExtrude:{
            return _makeExtrude(args);
        }
        case performFillet: {
            return _makeFillet(args);
        }
        case performChamfer:{
            return _makeChamfer(args);
        }
        case performRevolve:{
            return _makeRevolve(args);
        }
        default: {
            fprintf(stderr,"Hit default case in exec geom op exiting ... \n");
            exit(1);
        }
    }
}


NodeExpression* execMiscFunc(functionCategory category, std::vector<NodeExpression*>& args)
{
    switch (category.miscFuncCall){
        case print: {
            _print(args);
            return NULL;
        }
        case addShape: {
            return _addShape(args);
        }
        case createConnection: {
            return _createConnect(args);
        }
        case createLineTo: {
            return _createLineTo(args);
        }
        default: {
            fprintf(stderr, "execMiscFunc hit default case exiting ... \n");
            exit(1);
        }
    }
}



NodeExpression* execFunc(functionPtr* functionPtr, std::vector<NodeExpression*>& args)
{
    switch(functionPtr->funcType){
        case twoDim: { 
            return execTwoDim(functionPtr->functionVal, args);
        }
        case threeDim: {
            return execThreeDim(functionPtr->functionVal, args);
        }
        case booleanOperations: {
            return execBooleanOperations(functionPtr->functionVal, args);
        }
        case transformation: {
            return execTransformation(functionPtr->functionVal, args);
        }
        case geomOp: {
            return execGeomOp(functionPtr->functionVal, args);
        }
        case miscFunc: {
            return execMiscFunc(functionPtr->functionVal, args);
        }
        default: {
           fprintf(stderr, "Inside ExecFunc you are looking for function that does not exist how did you end up here ?\n"); 
           exit(1);
        }
    }
}

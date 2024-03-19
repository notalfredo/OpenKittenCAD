#include "Geom_TrimmedCurve.hxx"
#include "TopoDS_Wire.hxx"
#include "enumToString.hxx"
#include "gp_Ax1.hxx"
#include "gp_Trsf.hxx"
#include "node.hxx"
#include "functions.hxx"


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


#include <vtkStructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkExtractEdges.h>
#include <IVtkTools_ShapeDataSource.hxx>
#include "vtkInteractorStyle.h"
#include <vtkAutoInit.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>


static vtkNew<vtkRenderWindow> renwin;
static vtkNew<vtkRenderWindowInteractor> iren;
static vtkNew<vtkRenderer> ren;


void initViewer()
{
    renwin->AddRenderer(ren);

    vtkNew<vtkInteractorStyleTrackballCamera> istyle;
    
    iren->SetRenderWindow(renwin);
    iren->SetInteractorStyle(istyle);

    vtkNew<vtkStructuredGrid> grid;
    grid->SetDimensions(20, 20, 1);

    // Create points
    vtkNew<vtkPoints> points;

    // Generate points for the grid
    for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            points->InsertNextPoint(i, j, 0);
        }
    }

    // Set the points for the grid
    grid->SetPoints(points);


    vtkNew<vtkDataSetMapper> gridMapper;
    gridMapper->SetInputData(grid);  // Set the structured grid directly

    // Extract edges from the structured grid
    vtkNew<vtkExtractEdges> extractEdges;
    extractEdges->SetInputData(grid);

    // Create a mapper for the edges
    vtkNew<vtkPolyDataMapper> edgesMapper;
    edgesMapper->SetInputConnection(extractEdges->GetOutputPort());

    // Create an actor for the edges
    vtkNew<vtkActor> edgesActor;
    edgesActor->SetMapper(edgesMapper);
    ren->AddActor(edgesActor);
}


void startViewer()
{
    renwin->Render();
    iren->Start();
}


/* This function causes memmory leaks 
 * I am not 100% sure as to why. Something to do 
 * with VTK. Just creating oocSourceOne is enough
 * to create the leak
*/
void _addShape(const TopoDS_Shape& shapeToAdd)
{
    vtkNew<IVtkTools_ShapeDataSource> occSourceOne;
    occSourceOne->SetShape(new IVtkOCC_Shape(shapeToAdd));
    vtkNew<vtkPolyDataMapper> mapperOne;
    mapperOne->SetInputConnection(occSourceOne->GetOutputPort());
    vtkNew<vtkActor> actorOne;
    actorOne->SetMapper(mapperOne);
    ren->AddActor(actorOne);
}


void _print(double num)
{
    fprintf(stdout, "%f\n", num);
}


NodeShape* _makeSphere(Standard_Real radius)
{
    BRepPrimAPI_MakeSphere* sphere = new BRepPrimAPI_MakeSphere(radius);
    const TopoDS_Shape* shape = &sphere->Shape();

    NodeShape* me = newNodeShape(SPHERE);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(sphere);
    me->shape = shape;

    return me;
}

NodeShape* _makeCylinder(Standard_Real R1, Standard_Real H)
{
    BRepPrimAPI_MakeCylinder* cyl = new BRepPrimAPI_MakeCylinder(R1, H);
    const TopoDS_Shape* shape = &cyl->Shape();

    NodeShape* me = newNodeShape(CYLINDER);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(cyl);
    me->shape = shape;

    return me;
}


NodeShape* _makeBox(Standard_Real R1, Standard_Real R2, Standard_Real R3)
{
    BRepPrimAPI_MakeBox* box = new BRepPrimAPI_MakeBox(R1, R2, R3);
    const TopoDS_Shape* shape = &box->Shape();

    NodeShape* me = newNodeShape(BOX);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(box);
    me->shape = shape;

    return me;
}


NodeShape* _makeCone(Standard_Real R1, Standard_Real R2, Standard_Real H)
{
    BRepPrimAPI_MakeCone* cone = new BRepPrimAPI_MakeCone(R1, R2, H);
    const TopoDS_Shape* shape = &cone->Shape();

    NodeShape* me = newNodeShape(CONE);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(cone);
    me->shape = shape;

    return me;
}


NodeShape* _makeUnion(const TopoDS_Shape& lhs, const TopoDS_Shape& rhs)
{
    BRepAlgoAPI_Fuse* fuse = new BRepAlgoAPI_Fuse(lhs, rhs);
    const TopoDS_Shape* shape = &fuse->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(fuse);
    me->shape = shape;

    return me;
}

NodeShape* _makeDifference(const TopoDS_Shape& lhs, const TopoDS_Shape& rhs)
{
    BRepAlgoAPI_Cut* cut = new BRepAlgoAPI_Cut(lhs, rhs);
    const TopoDS_Shape* shape = &cut->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(cut);
    me->shape = shape;

    return me;
}


NodeShape* _makeIntersection(const TopoDS_Shape& lhs, const TopoDS_Shape& rhs)
{
    BRepAlgoAPI_Common* common = new BRepAlgoAPI_Common(lhs, rhs);
    const TopoDS_Shape* shape = &common->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(CUSTOM);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(common);
    me->shape = shape;

    return me;
}


NodeShape* _rotate(const TopoDS_Shape& currShape, double angle, OCCT_SHAPE shapeType,  gp_Ax1 xAxis)
{
    gp_Trsf transformation; 
    transformation.SetRotation(xAxis, angle);

    BRepBuilderAPI_Transform* rotation = new BRepBuilderAPI_Transform(currShape, transformation);
    const TopoDS_Shape* shape = &rotation->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(shapeType);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(rotation);
    me->shape = shape;

    return me;

}

NodeShape* _translate(const TopoDS_Shape& currShape, double x, double y, double z, OCCT_SHAPE shapeType)
{
    gp_Vec vector = gp_Vec(x, y, z);  
    gp_Trsf transformation;
    transformation.SetTranslation(vector);


    BRepBuilderAPI_Transform* translation = new BRepBuilderAPI_Transform(currShape, transformation);
    const TopoDS_Shape* shape = &translation->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(shapeType);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(translation);
    me->shape = shape;
    return me;


    return me;
}


NodeShape* _makeFace(const TopoDS_Wire* wire)
{
    BRepBuilderAPI_MakeFace* face = new BRepBuilderAPI_MakeFace(*wire);


    NodeShape* me = newNodeShape(FACE);
    me->brepShape = face;
    me->shape = &face->Shape();

    return me;
}


NodePoint* _makePoint(double x, double y, double z)
{
    gp_Pnt* point = new gp_Pnt(x, y, z);

    NodePoint* pointNode = newNodePoint();
    pointNode->point = point;

    return pointNode;
}


NodeEdge* _makeEdge(NodePoint* p1, NodePoint* p2)
{
    GC_MakeSegment mkSeg(*p1->point, *p2->point);
    Handle(Geom_TrimmedCurve) aSegment;
    if(mkSeg.IsDone()){
        aSegment = mkSeg.Value();
    }
    else {
        fprintf(stderr,
            "Unable to make edge between points {x: %f, y: %f, z: %f} and {x: %f, y: %f, z: %f} ... exiting ...\n",
            p1->point->X(), p1->point->Y(), p1->point->Z(),
            p2->point->X(), p2->point->Y(), p2->point->Z()
        );
        exit(1);
    }

    BRepBuilderAPI_MakeEdge* edge = new BRepBuilderAPI_MakeEdge(aSegment);    
    const TopoDS_Edge* result = &edge->Edge();

    NodeEdge* me = newNodeEdge();
    me->edgeType = type_edge;
    me->brepEdge = edge;
    me->edge = result;

    me->wireShape = NULL;
    me->brepWire = NULL;

    return me;
}


NodeEdge* _makeArc(NodePoint* p1, NodePoint* p2, NodePoint* p3)
{
    GC_MakeArcOfCircle mkArc(*p1->point, *p2->point, *p3->point);
    Handle(Geom_TrimmedCurve) aSegment;
    if(mkArc.IsDone()){
        aSegment = mkArc.Value();
    }
    else {
        fprintf(stderr,
            "Unable to make edge between points p1 {x: %f, y: %f, z: %f} and p3 {x: %f, y: %f, z: %f} that cross p2 {x: %f, y: %f, z: %f} ... exiting ...\n",
            p1->point->X(), p1->point->Y(), p1->point->Z(),
            p3->point->X(), p3->point->Y(), p3->point->Z(),
            p2->point->X(), p2->point->Y(), p2->point->Z()
        );
        exit(1);
    }


    BRepBuilderAPI_MakeEdge* edge = new BRepBuilderAPI_MakeEdge(aSegment);    
    const TopoDS_Edge* result = &edge->Edge();

    NodeEdge* me = newNodeEdge();
    me->edgeType = type_edge;
    me->brepEdge = edge;
    me->edge = result;

    me->wireShape = NULL;
    me->brepWire = NULL;

    return me;
}


NodeEdge* _connect(const TopoDS_Edge* edge1, const TopoDS_Edge* const edge2, const TopoDS_Edge* edge3)
{
    BRepBuilderAPI_MakeWire* brepWire = new BRepBuilderAPI_MakeWire(*edge1, *edge2, *edge3);

    const TopoDS_Wire* wireShape = &brepWire->Wire();


    NodeEdge* me = newNodeEdge();
    me->edge = NULL;
    me->brepEdge = NULL;
    me->edgeType = type_wire;

    me->brepWire = brepWire;
    me->wireShape = wireShape;

    return me;    
}


NodeEdge* _connect(const TopoDS_Wire* wire1, const TopoDS_Wire* wire2){
    BRepBuilderAPI_MakeWire* combinedWire = new BRepBuilderAPI_MakeWire();
    combinedWire->Add(*wire1);
    combinedWire->Add(*wire2);

    

    NodeEdge* me = newNodeEdge();
    me->edge = NULL;
    me->brepEdge = NULL;
    me->edgeType = type_wire;

    me->brepWire = combinedWire;
    me->wireShape = &combinedWire->Wire();
    return me;    

}


/*we need to pass some info on what type of shape we are. For now just assume we passed a wire*/
/*Also for now we just assume we can only mirror on the X axis */

NodeEdge* _mirror(const TopoDS_Wire* shape) {
    gp_Ax1 axis = gp::OX();
    gp_Trsf aTrsf;
    aTrsf.SetMirror(axis);


    TopoDS_Wire wire = TopoDS::Wire(BRepBuilderAPI_Transform(*shape, aTrsf).Shape());



    NodeEdge* me = newNodeEdge();
    me->edge = NULL;
    me->brepEdge = NULL;
    me->edgeType = type_wire;

    me->brepWire = new BRepBuilderAPI_MakeWire(wire);
    me->wireShape = &me->brepWire->Wire();
    
    return me;
}



functionPtr knownFunctions[] {
    {"sphere", makeSphere,  {.makeSphere = _makeSphere}},
    {"cone", makeCone,  {.makeCone = _makeCone}},
    {"cylinder", makeCylinder,  {.makeCylinder = _makeCylinder}},
    {"box", makeBox,  {.makeBox = _makeBox}},

    {"union", makeUnion,  {.makeUnion = _makeUnion}},
    {"difference", makeDifference,  {.makeDifference = _makeDifference}},
    {"intersection", makeIntersection,  {.makeIntersection = _makeIntersection}},
    {"makeFace", makeFace,  {.makeFace = _makeFace}},



    {"rotate", doRotate,  {.rotate = _rotate}},
    {"translate", doTranslate,  {.translate = _translate}},
    {"mirror", doMirror, {.mirror = _mirror}},
    

    {"dot", makePoint, {.makePoint = _makePoint}},
    {"line", makeEdge, {.makeEdge = _makeEdge}},
    {"arc", makeArc, {.makeArc = _makeArc}},
    {"connect", connect, {.connect = _connect}},


    {"print",  printDouble, {.println =  _print}},
    {"addShape", addShape,  {.addShapeToVTK = _addShape}}
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
NodeExpression* execFunc(functionPtr* functionPtr, std::vector<NodeExpression*>& args)
{
    switch(functionPtr->functionType){
        case printDouble: {
            if(args.size() != 1){
                fprintf(stderr, "print only takes 1 argument you passed %ld\n", args.size());
                exit(1);
            }

            switch(args[0]->nodeType){
                case DOUBLE:{
                    NodeNumber* numNode = static_cast<NodeNumber*>(args[0]);
                    _print(numNode->value);
                    return NULL;
                }
                case EDGE: {
                    NodeEdge* edgeNode = static_cast<NodeEdge*>(args[0]);        
                    switch(edgeNode->edgeType){
                        case type_edge: {
                            edgeNode->edge->DumpJson(std::cout);
                            return NULL;
                        }
                        case type_wire: {
                            edgeNode->wireShape->DumpJson(std::cout);
                            return NULL;
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
                    return NULL;

                }
                case SHAPE: {
                    NodeShape* shapeNode = static_cast<NodeShape*>(args[0]);
                    shapeNode->shape->DumpJson(std::cout);
                    return NULL;
                }
                default: {
                    fprintf(stderr, "You can only print numbers, points, edges, and shapes\n");
                    exit(1);
                }
            }
            return NULL;
        }

        case makeSphere: {
            NodeNumber* numNode = static_cast<NodeNumber*>(args[0]);
            return functionPtr->func.makeSphere(numNode->value);
        }
        case makeCone: {
            NodeNumber* one = static_cast<NodeNumber*>(args[0]);
            NodeNumber* two = static_cast<NodeNumber*>(args[1]);
            NodeNumber* three = static_cast<NodeNumber*>(args[2]);

            if(one == two){
                fprintf(stderr, "When creating cone top and bottom radius cannot be equal to each other");
                exit(1);
            }

            NodeExpression* result = functionPtr->func.makeCone(
                one->value,
                two->value,
                three->value
            );
            return result;
        }
        case makeCylinder: {
            NodeNumber* one = static_cast<NodeNumber*>(args[0]);
            NodeNumber* two = static_cast<NodeNumber*>(args[1]);

            return functionPtr->func.makeCylinder(one->value, two->value);
        }
        case addShape: {

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
            _addShape(*sphere->shape);
            return NULL;
        }
        case makeUnion: {
            NodeShape* lhs = static_cast<NodeShape*>(args[0]);
            NodeShape* rhs = static_cast<NodeShape*>(args[1]);

            return functionPtr->func.makeUnion(*lhs->shape, *rhs->shape);
        }
        case makeDifference: {
            NodeShape* lhs = static_cast<NodeShape*>(args[0]);
            NodeShape* rhs = static_cast<NodeShape*>(args[1]);

            return functionPtr->func.makeDifference(*lhs->shape, *rhs->shape);
        }
        case makeIntersection: {
            NodeShape* lhs = static_cast<NodeShape*>(args[0]);
            NodeShape* rhs = static_cast<NodeShape*>(args[1]);


            

            return functionPtr->func.makeIntersection(*lhs->shape, *rhs->shape);
        }
        case makeBox: {
            NodeNumber* one = static_cast<NodeNumber*>(args[0]);
            NodeNumber* two = static_cast<NodeNumber*>(args[1]);
            NodeNumber* three = static_cast<NodeNumber*>(args[2]);

            return functionPtr->func.makeBox(one->value, two->value, three->value);
        }
        case doRotate: {
            NodeShape* one = static_cast<NodeShape*>(args[0]);
            NodeArray* two = static_cast<NodeArray*>(args[1]);

            int length = getExpressionLength(two->array);
            if( length != 3 ){
                fprintf(stderr, "Array argument to rotation must be length 3 ... exiting ...\n");
            }
            else if(!checkAllExprTypes(two->array, DOUBLE)){
                fprintf(stderr, "The elements for the array in rotation must all evaluate to a double ... exiting ...\n"); 
            }

            double numOne = static_cast<NodeNumber*>(two->array)->value;
            double numTwo = static_cast<NodeNumber*>(two->array->nextExpr)->value;
            double numThree = static_cast<NodeNumber*>(two->array->nextExpr->nextExpr)->value;


            NodeShape* newShape = NULL;
            newShape = functionPtr->func.rotate(*one->shape, numOne, one->shapeType, gp::OX());
            newShape = functionPtr->func.rotate(*newShape->shape, numTwo, one->shapeType, gp::OY());
            newShape = functionPtr->func.rotate(*newShape->shape, numThree, one->shapeType, gp::OZ());

            return newShape;
        }
        case doTranslate: {
            NodeShape* one = static_cast<NodeShape*>(args[0]);
            NodeArray* two = static_cast<NodeArray*>(args[1]);

            int length = getExpressionLength(two->array);
            if( length != 3 ){
                fprintf(stderr, "Array argument to rotation must be length 3 ... exiting ...\n");
            }
            else if(!checkAllExprTypes(two->array, DOUBLE)){
                fprintf(stderr, "The elements for the array in rotation must all evaluate to a double ... exiting ...\n"); 
            }

            double xTrans = static_cast<NodeNumber*>(two->array)->value;
            double yTrans = static_cast<NodeNumber*>(two->array->nextExpr)->value;
            double zTrans = static_cast<NodeNumber*>(two->array->nextExpr->nextExpr)->value;

            return functionPtr->func.translate(*one->shape, xTrans, yTrans, zTrans, one->shapeType);
        }
        case makePoint: {
            NodeArray* arrNode = static_cast<NodeArray*>(args[0]);

            int length = getExpressionLength(arrNode->array);
            if( length != 3 ){
                fprintf(stderr, "Array argument to rotation must be length 3 ... exiting ...\n");
            }
            else if(!checkAllExprTypes(arrNode->array, DOUBLE)){
                fprintf(stderr, "All values inside array for making point must be double ... exiting ...\n"); 
            }
            

            double x = static_cast<NodeNumber*>(arrNode->array)->value;
            double y = static_cast<NodeNumber*>(arrNode->array->nextExpr)->value;
            double z = static_cast<NodeNumber*>(arrNode->array->nextExpr->nextExpr)->value;


            return functionPtr->func.makePoint(x, y, z);
        }
        case makeEdge: {
            if(args.size() != 2){
                fprintf(stderr, "When creating line must provide 2 points");
                exit(1);
            }
            else if(args[0]->nodeType != POINT || args[1]->nodeType != POINT){
                fprintf(stderr, "When creating an edge it must be between two points p1: %s, p2: %s\n",
                    nodeTypeToString(args[0]->nodeType),
                    nodeTypeToString(args[1]->nodeType)
                ); 
                exit(1);
            }

            NodePoint* p1 = static_cast<NodePoint*>(args[0]);
            NodePoint* p2 = static_cast<NodePoint*>(args[1]);

            return functionPtr->func.makeEdge(p1, p2);
        }
        case makeArc: {
            if(args.size() != 3){
                fprintf(stderr, "When creating arc must provide 3 points");
                exit(1);
            }
            else if(args[0]->nodeType != POINT || args[1]->nodeType != POINT || args[2]->nodeType != POINT){
                fprintf(stderr, "When creating an edge it must be between three points p: %s, p: %s, p: %s\n",
                    nodeTypeToString(args[0]->nodeType),
                    nodeTypeToString(args[1]->nodeType),
                    nodeTypeToString(args[2]->nodeType)
                ); 
                exit(1);
            }

            NodePoint* p1 = static_cast<NodePoint*>(args[0]);
            NodePoint* p2 = static_cast<NodePoint*>(args[1]);
            NodePoint* p3 = static_cast<NodePoint*>(args[2]);

            return functionPtr->func.makeArc(p1, p2, p3);
        }
        case connect:{
            if(args.size() != 3 && args.size() != 2){
                fprintf(stderr, "When connecting edges must connect only 2 or 3 at a time, you passed %ld\n", args.size());
                exit(1);
            }

            if(args.size() == 3 ){
                NodeEdge* e1 = static_cast<NodeEdge*>(args[0]);
                NodeEdge* e2 = static_cast<NodeEdge*>(args[1]);
                NodeEdge* e3 = static_cast<NodeEdge*>(args[2]);

                return functionPtr->func.connect(e1->edge, e2->edge, e3->edge);
            }
            else {
                NodeEdge* e1 = static_cast<NodeEdge*>(args[0]);
                NodeEdge* e2 = static_cast<NodeEdge*>(args[1]);

                return _connect(e1->wireShape, e2->wireShape);
            }
        }
        case doMirror:{
            NodeEdge* e1 = static_cast<NodeEdge*>(args[0]);

            return functionPtr->func.mirror(e1->wireShape);
        }
        case makeFace: {
            NodeEdge* e1 = static_cast<NodeEdge*>(args[0]);

            return functionPtr->func.makeFace(e1->wireShape);
        }
        default: {
           fprintf(stderr, "Inside ExecFunc you are looking for function that does not exist how did you end up here ?\n"); 
           exit(1);
        }
    }
}

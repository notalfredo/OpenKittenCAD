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


typedef enum occt_boolean {
    FUSE,
    DIFFERENCE,
    INTERSECTION
}OCCT_BOOLEAN;

typedef enum occt_transformation {
    ROTATE,
    TRANSLATION
}OCCT_TRANSFORMATION;



NodePoint* _makePoint(std::vector<NodeExpression*>& args);


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
void _addShapeVtk(const TopoDS_Shape& shapeToAdd)
{
    vtkNew<IVtkTools_ShapeDataSource> occSourceOne;
    occSourceOne->SetShape(new IVtkOCC_Shape(shapeToAdd));
    vtkNew<vtkPolyDataMapper> mapperOne;
    mapperOne->SetInputConnection(occSourceOne->GetOutputPort());
    vtkNew<vtkActor> actorOne;
    actorOne->SetMapper(mapperOne);
    ren->AddActor(actorOne);
}

void dumpArgumentsAndCorrectArguments(std::vector<std::vector<PARAM_INFO>>& paramInfo, std::vector<NodeExpression*>& args, const char* functionName){
    
    fprintf(stderr, "You invoked %s with incompatible arguments\n", functionName); 
    fprintf(stderr, "Valid arguments are: \n");

    for(int i = 0; i < paramInfo.size(); i++){
        fprintf(stderr, "(");
       
        for(int j = 0; j < paramInfo[i].size(); j++){
            fprintf(stderr, "%s: %s, ", 
                paramInfo[i][j].name.c_str(),
                nodeTypeToString(paramInfo[i][j].type)
            );
        }

        fprintf(stderr, ")\n");
    }

    fprintf(stderr, "You invoked with: \n");

    fprintf(stderr, "(");
    for(int i = 0; i < args.size(); i++){
        fprintf(stderr, "%s",  nodeTypeToString(args[i]->nodeType));
    }
    fprintf(stderr, ") ... exiting ...\n");
    exit(1);
}



int validateFunctionArguments(std::vector<std::vector<PARAM_INFO>>& paramInfo, std::vector<NodeExpression*>& args){

    for(int i = 0; i < paramInfo.size(); i++){
        if(args.size() != paramInfo[i].size()){
            continue;
        }

        int contFlag = 0;

        for(int j = 0; j < paramInfo[i].size(); j++){
            if(args[j]->nodeType != paramInfo[i][j].type){
               contFlag = 1; 
            }
        }

        if (contFlag) {
            continue;
        }

        return i;
    }

    return -1;
}


BRepPrimAPI_MakeSphere* _validateSphere(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {DOUBLE, "radius"} },
        { {DOUBLE, "radius"}, {DOUBLE, "angle"} },
        { {DOUBLE, "radius"}, {DOUBLE, "angleOne"}, {DOUBLE, "angleTwo"} },
        { {DOUBLE, "radius"}, {DOUBLE, "angleOne"}, {DOUBLE, "angleTwo"}, {DOUBLE, "angle"} },
    };



    if((args.size() < 1) && (args.size() > 4)){
        fprintf(stderr, "Sphere can only be invoked with 1-4 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    int argIndex = validateFunctionArguments(validParams, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(validParams, args, "sphere");  
    }


    switch(argIndex){
        case 0: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            return new BRepPrimAPI_MakeSphere(arg1->value);
        }
        case 1: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            return new BRepPrimAPI_MakeSphere(arg1->value, arg2->value);
        }
        case 2: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 

            if(!((0 < (arg3->value - arg2->value)) && ((arg3->value - arg2->value) < 2*3.14))) {

                fprintf(stderr, 
                    "The angles angle1, angle2 must follow the relation 0 < a2 - a1 < 2*PI, (%lf - %lf = %lf) ... exiting ...\n",
                    arg3->value, arg2->value,
                    (arg3->value - arg2->value)
                );
                exit(1);
            }

            return new BRepPrimAPI_MakeSphere(arg1->value, arg2->value, arg3->value);
        }
        case 3: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 
            NodeNumber* arg4 = static_cast<NodeNumber*>(args[3]); 
            return new BRepPrimAPI_MakeSphere(arg1->value, arg2->value, arg3->value, arg4->value);
        }

        default: {
            fprintf(stderr, "Hit the default case in _validateSphere ... this should not be possible \n");
            return NULL;
        }
    }
}


BRepPrimAPI_MakeCone* _validateCone(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {DOUBLE, "radius1"}, {DOUBLE, "radius2"}, {DOUBLE, "height"} },
        { {DOUBLE, "radius1"}, {DOUBLE, "radius2"}, {DOUBLE, "height"}, {DOUBLE, "angle"} },
    };



    if((args.size() < 3) && (args.size() > 4)){
        fprintf(stderr, "Cone can only be invoked with 3-4 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    int argIndex = validateFunctionArguments(validParams, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(validParams, args, "sphere");  
    }


    switch(argIndex){
        case 0: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 

            if(arg1 == arg2){
                fprintf(stderr, "Cone top radius cannot equal bottom radius you passed (%lf, %lf)", arg1->value, arg2->value);
                exit(1);
            }


            return new BRepPrimAPI_MakeCone(arg1->value, arg2->value, arg3->value);
        }
        case 1: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 
            NodeNumber* arg4 = static_cast<NodeNumber*>(args[3]); 

            if(arg1 == arg2){
                fprintf(stderr, "Cone top radius cannot equal bottom radius you passed (%lf, %lf)", arg1->value, arg2->value);
                exit(1);
            }

            return new BRepPrimAPI_MakeCone(arg1->value, arg2->value, arg3->value, arg4->value);
        }
        default: {
            fprintf(stderr, "Hit the default case in _validateCone ... this should not be possible \n");
            return NULL;
        }
    }
}


BRepPrimAPI_MakeCylinder* _validateCylinder(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {DOUBLE, "radius"}, {DOUBLE, "height"} },
        { {DOUBLE, "radius"}, {DOUBLE, "height"}, {DOUBLE, "angle"} },
    };


    if((args.size() < 2) && (args.size() > 3)){
        fprintf(stderr, "Cone can only be invoked with 2-4 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    int argIndex = validateFunctionArguments(validParams, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(validParams, args, "sphere");  
    }


    switch(argIndex){
        case 0: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 

            return new BRepPrimAPI_MakeCylinder(arg1->value, arg2->value);
        }
        case 1: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 

            return new BRepPrimAPI_MakeCylinder(arg1->value, arg2->value, arg3->value);
        }
        default: {
            fprintf(stderr, "Hit the default case in _validateCylinder ... this should not be possible \n");
            return NULL;
        }
    }
}


BRepPrimAPI_MakeBox* _validateBox(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {DOUBLE, "dx"}, {DOUBLE, "dy"}, {DOUBLE, "dz"} },
        { {POINT, "point"}, {DOUBLE, "dx"}, {DOUBLE, "dy"}, {DOUBLE, "dz"} },
        { {POINT, "point1"}, {POINT, "point2"} },
    };


    if((args.size() < 3) && (args.size() > 4)){
        fprintf(stderr, "Box can only be invoked with 3-4 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    int argIndex = validateFunctionArguments(validParams, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(validParams, args, "sphere");  
    }


    switch(argIndex){
        case 0: {
            NodeNumber* arg1 = static_cast<NodeNumber*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 

            return new BRepPrimAPI_MakeBox(arg1->value, arg2->value, arg3->value);
        }
        case 1: {
            NodePoint* arg1 = static_cast<NodePoint*>(args[0]); 
            NodeNumber* arg2 = static_cast<NodeNumber*>(args[1]); 
            NodeNumber* arg3 = static_cast<NodeNumber*>(args[2]); 
            NodeNumber* arg4 = static_cast<NodeNumber*>(args[3]); 

            return new BRepPrimAPI_MakeBox(*arg1->point, arg2->value, arg3->value, arg4->value);
        }
        case 2: {
            NodePoint* arg1 = static_cast<NodePoint*>(args[0]); 
            NodePoint* arg2 = static_cast<NodePoint*>(args[1]); 

            return new BRepPrimAPI_MakeBox(*arg1->point, *arg2->point);
        }
        default: {
            fprintf(stderr, "Hit the default case in _validateCylinder ... this should not be possible \n");
            return NULL;
        }
    }
}


BRepAlgoAPI_BooleanOperation* _validateBoolean(std::vector<NodeExpression*>& args, OCCT_BOOLEAN booleanType)
{
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {SHAPE, "lhs"}, {SHAPE, "rhs"} },
    };

    if( args.size() != 2 ){
        fprintf(stderr, "Boolean operations can only be invoked with 2 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    int index = validateFunctionArguments(validParams, args);

    if(index == -1){
        switch(booleanType){
            case FUSE: {
                dumpArgumentsAndCorrectArguments(validParams, args, "FUSE");
            }
            case DIFFERENCE: {
                dumpArgumentsAndCorrectArguments(validParams, args, "difference");
            }
            case INTERSECTION: {
                dumpArgumentsAndCorrectArguments(validParams, args, "intersection");
            }
        }
    }


    NodeShape* lhs = static_cast<NodeShape*>(args[0]);
    NodeShape* rhs = static_cast<NodeShape*>(args[1]);

    switch(booleanType){
        case FUSE: {
            return new BRepAlgoAPI_Fuse(*lhs->shape, *rhs->shape);
        }
        case DIFFERENCE: {
            return new BRepAlgoAPI_Cut(*lhs->shape, *rhs->shape);
        }
        case INTERSECTION: {
            return new BRepAlgoAPI_Common(*lhs->shape, *rhs->shape);
        }
    }

    fprintf(stderr, "Hit out of flow validate boolean\n");
    exit(1);
}


void _validateRotateTranslate(std::vector<NodeExpression*>& args){
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {SHAPE, "shape"}, {ARRAY, "rotation"} },
    };

    if( args.size() != 2 ){
        fprintf(stderr, "Rotation operation can only be invoked with 2 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    validateFunctionArguments(validParams, args);

    int length = getExpressionLength(static_cast<NodeArray*>(args[1])->array);
    
    if( (length < 1) || (length > 3) ){
        fprintf(stderr, "Valid for arrays for Rotate or Translate must have length 1-3, you gave array of size %d\n", length);
        exit(1);
    }
    else if(!checkAllExprTypes(static_cast<NodeArray*>(args[1])->array, DOUBLE)){
        fprintf(stderr, "The elements for the array in rotation must all evaluate to a double ... exiting ...\n"); 
    }
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


gp_Pnt* _validatePoint(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> paramInfo = {
        { {ARRAY, "point"} }
    };

    int index = validateFunctionArguments(paramInfo, args);

    if(index == -1){
        dumpArgumentsAndCorrectArguments(paramInfo, args, "point");
    }


    int length = getExpressionLength(static_cast<NodeArray*>(args[0])->array);


    double x, y ,z;

    switch(index){
        case 1: {
            if( length != 3 ){
                fprintf(stderr, "Array argument to point must be length 3 ... exiting ...\n");
            }
            else if(!checkAllExprTypes(static_cast<NodeArray*>(args[0])->array, DOUBLE)){
                fprintf(stderr, "All values inside array for making point must be double ... exiting ...\n"); 
            }


            NodeArray* arrNode = static_cast<NodeArray*>(args[0]);
            x = static_cast<NodeNumber*>(arrNode->array)->value;
            y = static_cast<NodeNumber*>(arrNode->array->nextExpr)->value;
            z = static_cast<NodeNumber*>(arrNode->array->nextExpr->nextExpr)->value;
            break;
        }
        default :{
            fprintf(stderr, "Hit default case in validating point ... exiting ... \n");
            exit(1);
        }
    }

    return new gp_Pnt(x, y, z);
}


BRepBuilderAPI_MakeEdge* _validateEdge(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> paramInfo = {
        { {POINT, "point1"}, {POINT, "point2"} },
        { {ARRAY, "point1"}, {ARRAY, "point2"} }
    };

    int index = validateFunctionArguments(paramInfo, args);


    if(index == -1){
        dumpArgumentsAndCorrectArguments(paramInfo, args, "edge") ;
    }


    GC_MakeSegment* mkSeg = NULL;
    NodePoint* point1 = NULL;
    NodePoint* point2 = NULL;

    switch(index){
        case 0: {
            point1 = static_cast<NodePoint*>(args[0]);
            point2 = static_cast<NodePoint*>(args[1]);
            mkSeg = new GC_MakeSegment(*point1->point, *point2->point);
            break;
        }
        case 1: {
            NodeArray* arrOne = static_cast<NodeArray*>(args[0]);
            NodeArray* arrTwo = static_cast<NodeArray*>(args[1]);

            if(!checkAllExprTypes(arrOne->array, DOUBLE) && (getExpressionLength(arrOne->array) != 3)){
                fprintf(stderr, "First point of edge must be length 3 and must be all double\n");
                exit(1);
            }
            if(!checkAllExprTypes(arrTwo->array, DOUBLE) && (getExpressionLength(arrTwo->array) != 3)){
                fprintf(stderr, "Second point of edge must be length 3 and must be all double\n");
                exit(1);
            }

            std::vector<NodeExpression*> first {arrOne};
            std::vector<NodeExpression*> second {arrTwo};

            point1 = _makePoint(first);
            point2 = _makePoint(second);
            mkSeg = new GC_MakeSegment(*point1->point, *point2->point);
            break;
        }
        default: {
            fprintf(stderr, "Hit default case in validate edge exiting ...");
            exit(1);
        }
    }

    Handle(Geom_TrimmedCurve) aSegment;
    if(mkSeg->IsDone()){
        aSegment = mkSeg->Value();
    }
    else {
        fprintf(stderr, "Unable to make edge between points {x: %f, y: %f, z: %f} and {x: %f, y: %f, z: %f} ... exiting ...\n", point1->point->X(), point1->point->Y(), point1->point->Z(), point2->point->X(), point2->point->Y(), point2->point->Z());
        exit(1);
    }

    return new BRepBuilderAPI_MakeEdge(aSegment);    
}


BRepBuilderAPI_MakeEdge* _validateArc(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> paramInfo = {
        { {POINT, "point1"}, {POINT, "point2"}, {POINT, "point3"} },
        { {ARRAY, "point1"}, {ARRAY, "point2"}, {ARRAY, "point3"}}
    };

    int index = validateFunctionArguments(paramInfo, args);
    if(index == -1){
        dumpArgumentsAndCorrectArguments(paramInfo, args, "arc");
    }

    
    GC_MakeArcOfCircle* mkArc = NULL;
    NodePoint* p1 = NULL;
    NodePoint* p2 = NULL;
    NodePoint* p3 = NULL;

    switch(index){
        case 0: {
            p1 = static_cast<NodePoint*>(args[0]);
            p2 = static_cast<NodePoint*>(args[1]);
            p3 = static_cast<NodePoint*>(args[2]);

            mkArc = new GC_MakeArcOfCircle(*p1->point, *p2->point, *p3->point);
            break;
        }
        case 1: {
            NodeArray* arrOne = static_cast<NodeArray*>(args[0]);
            NodeArray* arrTwo = static_cast<NodeArray*>(args[1]);
            NodeArray* arrThree = static_cast<NodeArray*>(args[2]);

            if(!checkAllExprTypes(arrOne->array, DOUBLE) && (getExpressionLength(arrOne->array) != 3)){
                fprintf(stderr, "First point of edge must be length 3 and must be all double\n");
                exit(1);
            }
            if(!checkAllExprTypes(arrTwo->array, DOUBLE) && (getExpressionLength(arrTwo->array) != 3)){
                fprintf(stderr, "Second point of edge must be length 3 and must be all double\n");
                exit(1);
            }
            if(!checkAllExprTypes(arrThree->array, DOUBLE) && (getExpressionLength(arrThree->array) != 3)){
                fprintf(stderr, "Thrid point of edge must be length 3 and must be all double\n");
                exit(1);
            }
            std::vector<NodeExpression*> first {arrOne};
            std::vector<NodeExpression*> second {arrTwo};
            std::vector<NodeExpression*> thrid {arrThree};

            p1 = _makePoint(first);
            p2 = _makePoint(second);
            p3 = _makePoint(thrid);

            mkArc = new GC_MakeArcOfCircle(*p1->point, *p2->point, *p3->point);
            break;
        }
        default: {
            fprintf(stderr, "When validating arc hit default case exiting ... \n");
            exit(1);
        }
    }

    Handle(Geom_TrimmedCurve) aSegment;
    if(mkArc->IsDone()){
        aSegment = mkArc->Value();
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

    return new BRepBuilderAPI_MakeEdge(aSegment);    
}


void _validateConnect(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {EDGE, "edge1"}, {EDGE, "edge2"} },
        { {EDGE, "edge1"}, {EDGE, "edge2"}, {EDGE, "edge3"} }
    };

    
    int argIndex = validateFunctionArguments(param, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "connect");
    }


}


NodeShape* _makeSphere(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeSphere* sphere = _validateSphere(args);
    const TopoDS_Shape* shape = &sphere->Shape();

    NodeShape* me = newNodeShape(SPHERE);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(sphere);
    me->shape = shape;

    return me;
}


NodeShape* _makeCone(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeCone* cone = _validateCone(args);
    const TopoDS_Shape* shape = &cone->Shape();

    NodeShape* me = newNodeShape(CONE);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(cone);
    me->shape = shape;

    return me;
}


NodeShape* _makeCylinder(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeCylinder* cyl = _validateCylinder(args);
    const TopoDS_Shape* shape = &cyl->Shape();

    NodeShape* me = newNodeShape(CYLINDER);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(cyl);
    me->shape = shape;

    return me;
}


NodeShape* _makeBox(std::vector<NodeExpression*>& args)
{
    BRepPrimAPI_MakeBox* box = _validateBox(args);
    const TopoDS_Shape* shape = &box->Shape();

    NodeShape* me = newNodeShape(BOX);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(box);
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
    _addShapeVtk(*sphere->shape);
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
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(fuse);
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
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(cut);
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
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(common);
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



NodeShape* _rotate(std::vector<NodeExpression*>& args)
{
    _validateRotateTranslate(args);


    NodeShape* one = static_cast<NodeShape*>(args[0]);
    NodeArray* two = static_cast<NodeArray*>(args[1]);


    double numOne = static_cast<NodeNumber*>(two->array)->value;
    double numTwo = static_cast<NodeNumber*>(two->array->nextExpr)->value;
    double numThree = static_cast<NodeNumber*>(two->array->nextExpr->nextExpr)->value;


    gp_Trsf xRotation, yRotation, zRotation; 
    xRotation.SetRotation(gp::OX(), numOne);
    yRotation.SetRotation(gp::OY(), numTwo);
    zRotation.SetRotation(gp::OZ(), numThree);



    BRepBuilderAPI_Transform* xRot = new BRepBuilderAPI_Transform(*one->shape, xRotation);
    BRepBuilderAPI_Transform* xyRot = new BRepBuilderAPI_Transform(xRot->Shape(), yRotation);
    BRepBuilderAPI_Transform* xyzRot = new BRepBuilderAPI_Transform(xyRot->Shape(), zRotation);


    const TopoDS_Shape* shape = &xyzRot->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(one->shapeType);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(xyzRot);
    me->shape = shape;

    return me;
}

NodeShape* _translate(std::vector<NodeExpression*>& args)
{
    _validateRotateTranslate(args);


    NodeShape* one = static_cast<NodeShape*>(args[0]);
    NodeArray* two = static_cast<NodeArray*>(args[1]);


    double x = static_cast<NodeNumber*>(two->array)->value;
    double y = static_cast<NodeNumber*>(two->array->nextExpr)->value;
    double z = static_cast<NodeNumber*>(two->array->nextExpr->nextExpr)->value;


    gp_Trsf transformation;
    transformation.SetTranslation(gp_Vec(x, y, z));


    BRepBuilderAPI_Transform* translation = new BRepBuilderAPI_Transform(*one->shape, transformation);
    const TopoDS_Shape* shape = &translation->Shape();

    if(shape->IsNull()){
        fprintf(stderr, "Fuse is null exiting...\n");
        exit(1);
    }

    NodeShape* me = newNodeShape(one->shapeType);
    me->brepShape = static_cast<BRepBuilderAPI_MakeShape*>(translation);
    me->shape = shape;
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
    {"sphere", makeSphere,  {.makeSphere = NULL}},
    {"cone", makeCone, {.makeCone = NULL}},
    {"cylinder", makeCylinder,  {.makeCylinder = NULL}},
    {"box", makeBox,  {.makeBox = NULL}},


    {"union", makeUnion,  {.makeUnion = NULL}},
    {"difference", makeDifference,  {.makeDifference = NULL}},
    {"intersection", makeIntersection,  {.makeIntersection = NULL}},
    {"makeFace", makeFace,  {.makeFace = NULL}},



    {"rotate", doRotate,  {.rotate = NULL}},
    {"translate", doTranslate,  {.translate = NULL}},
    {"mirror", doMirror, {.mirror = NULL}},
    

    {"dot", makePoint, {.makePoint = NULL}},
    {"line", makeEdge, {.makeEdge = NULL}},
    {"arc", makeArc, {.makeArc = NULL}},

    {"connect", connect, {.connect = NULL}},


    {"print",  printDouble, {.println =  NULL}},
    {"addShape", addShape,  {.addShapeToVTK = NULL}}
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
            _print(args);
            return NULL;
        }
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
        case addShape: {
            return _addShape(args);
        }
        case makeUnion: {
            return _makeUnion(args);
        }
        case makeDifference: {
            return _makeDifference(args);
        }
        case makeIntersection: {
            return _makeIntersection(args);
        }
        case doRotate: {
            return _rotate(args);
        }
        case doTranslate: {
            return _translate(args); 
        }
        case makePoint: {
            return _makePoint(args);
        }
        case makeEdge: {
            return _makeEdge(args);
        }
        case makeArc: {
            return _makeArc(args);
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

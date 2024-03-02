#include "node.hxx"
#include "functions.hxx"


#include "Standard_TypeDef.hxx"
#include "TopoDS_Shape.hxx"
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include "BRepBuilderAPI_MakeShape.hxx"
#include <BRepPrimAPI_MakeCylinder.hxx>


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



functionPtr knownFunctions[] {
    {"sphere", makeSphere,  {.makeSphere = _makeSphere}},
    {"cone", makeCone,  {.makeCone = _makeCone}},
    {"cylinder", makeCylinder,  {.makeCylinder = _makeCylinder}},
    {"box", makeBox,  {.makeBox = _makeBox}},
    

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
            NodeNumber* numNode = static_cast<NodeNumber*>(args[0]);
            functionPtr->func.println(numNode->value);
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
            NodeShape* sphere = static_cast<NodeShape*>(args[0]);
            _addShape(*sphere->shape);
            return NULL;
        }
        case makeBox: {
            NodeNumber* one = static_cast<NodeNumber*>(args[0]);
            NodeNumber* two = static_cast<NodeNumber*>(args[1]);
            NodeNumber* three = static_cast<NodeNumber*>(args[2]);

            return functionPtr->func.makeBox(one->value, two->value, three->value);
        }
        default: {
           fprintf(stderr, "Inside ExecFunc you are looking for function that does not exist how did you end up here ?"); 
           exit(1);
        }
    }
}

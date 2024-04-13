#ifndef VALIDATEFUNC_HXX
#define VALIDATEFUNC_HXX

#include "BRepPrimAPI_MakeRevol.hxx"
#include "BRepBuilderAPI_ModifyShape.hxx"
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
#include <TopExp_Explorer.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>


typedef struct paramInfo {
    nodeType type;
    std::string name;
} PARAM_INFO;

typedef enum occt_boolean {
    FUSE,
    DIFFERENCE,
    INTERSECTION
}OCCT_BOOLEAN;


typedef enum occt_transformation {
    ROTATION,
    TRANSLATION
}OCCT_TRANSFORMATION;


extern void dumpArgumentsAndCorrectArguments(std::vector<std::vector<PARAM_INFO>>& paramInfo, std::vector<NodeExpression*>& args, const char* functionName);

extern int validateFunctionArguments(std::vector<std::vector<PARAM_INFO>>& paramInfo, std::vector<NodeExpression*>& args);


extern BRepPrimAPI_MakeSphere* _validateSphere(std::vector<NodeExpression*>& args);


extern BRepPrimAPI_MakeCone* _validateCone(std::vector<NodeExpression*>& args);


extern BRepPrimAPI_MakeCylinder* _validateCylinder(std::vector<NodeExpression*>& args);


extern BRepPrimAPI_MakeBox* _validateBox(std::vector<NodeExpression*>& args);


extern BRepAlgoAPI_BooleanOperation* _validateBoolean(std::vector<NodeExpression*>& args, OCCT_BOOLEAN booleanType);


extern BRepBuilderAPI_ModifyShape* _validateRotateTranslate(std::vector<NodeExpression*>& args, OCCT_TRANSFORMATION transformation, OCCT_SHAPE& shapeType);


extern gp_Pnt* _validatePoint(std::vector<NodeExpression*>& args);


extern BRepBuilderAPI_MakeEdge* _validateEdge(std::vector<NodeExpression*>& args);


extern BRepBuilderAPI_MakeEdge* _validateArc(std::vector<NodeExpression*>& args);


extern BRepBuilderAPI_MakeWire* _validateConnect(std::vector<NodeExpression*>& args);


extern void _validateMirror(std::vector<NodeExpression*>& args);


extern BRepBuilderAPI_MakeFace* _validateFace(std::vector<NodeExpression*>& args);


extern NodeShape* _makeSphere(std::vector<NodeExpression*>& args);


extern BRepPrimAPI_MakePrism* _validateExtrude(std::vector<NodeExpression*>& args);


extern BRepFilletAPI_MakeFillet* _validateFillet(std::vector<NodeExpression*>& args, OCCT_SHAPE& shapeType);


extern BRepFilletAPI_MakeChamfer* _validateChamfer(std::vector<NodeExpression*>& args, OCCT_SHAPE& shapeType);


extern BRepPrimAPI_MakeTorus* _validateTorus(std::vector<NodeExpression*>& args);


extern NodeArray* _validateLineTo(std::vector<NodeExpression*>& args);


extern BRepPrimAPI_MakeRevol* _validateRevol(std::vector<NodeExpression*> & args);
#endif

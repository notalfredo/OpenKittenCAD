#include "validateFunc.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepPrimAPI_MakePrism.hxx"
#include "BRepPrimAPI_MakeRevol.hxx"
#include "gp_Pnt.hxx"
#include "node.hxx"


NodePoint* _makePoint(std::vector<NodeExpression*>& args);


void dumpArgumentsAndCorrectArguments(
        std::vector<std::vector<PARAM_INFO>>& paramInfo,
        std::vector<NodeExpression*>& args,
        const char* functionName
)
{
    
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
        fprintf(stderr, "%s\t",  nodeTypeToString(args[i]->nodeType));
    } fprintf(stderr, ") ... exiting ...\n"); exit(1);
}


int validateFunctionArguments(std::vector<std::vector<PARAM_INFO>>& paramInfo, std::vector<NodeExpression*>& args)
{

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


BRepBuilderAPI_ModifyShape* _validateRotateTranslate(std::vector<NodeExpression*>& args, OCCT_TRANSFORMATION transformation, OCCT_SHAPE& shapeType)
{
    std::vector<std::vector<PARAM_INFO>> validParams {
        { {SHAPE, "shape"}, {ARRAY, "rotation"} },
    };

    if( args.size() != 2 ){
        fprintf(stderr, "Rotation operation can only be invoked with 2 arguments you passed %ld ... exiting ...", args.size());
        exit(1);
    }

    int index = validateFunctionArguments(validParams, args);
    if(index == -1){
        switch(transformation){
            case ROTATION: {
                dumpArgumentsAndCorrectArguments(validParams, args, "rotate");
            }
            case TRANSLATION: {
                dumpArgumentsAndCorrectArguments(validParams, args, "translation");
            }
        }
    }


    switch(index){
        case 0:{
            int length = getExpressionLength(static_cast<NodeArray*>(args[1])->array);
            
            if( (length < 1) || (length > 3) ){
                fprintf(stderr, "Valid for arrays for Rotate or Translate must have length 1-3, you gave array of size %d\n", length);
                exit(1);
            }
            else if(!checkAllExprTypes(static_cast<NodeArray*>(args[1])->array, DOUBLE)){
                fprintf(stderr, "The elements for the array in rotation must all evaluate to a double ... exiting ...\n"); 
            }
            switch(transformation){
                case ROTATION: {

                    NodeShape* one = static_cast<NodeShape*>(args[0]);
                    NodeArray* two = static_cast<NodeArray*>(args[1]);


                    double numOne = static_cast<NodeNumber*>(two->array)->value;
                    double numTwo = static_cast<NodeNumber*>(two->array->nextExpr)->value;
                    double numThree = static_cast<NodeNumber*>(two->array->nextExpr->nextExpr)->value;

                    gp_Trsf xRotation, yRotation, zRotation; 
                    xRotation.SetRotation(gp::OX(), numOne);
                    yRotation.SetRotation(gp::OY(), numTwo);
                    zRotation.SetRotation(gp::OZ(), numThree);


                    shapeType = one->shapeType;
                    return new BRepBuilderAPI_Transform(
                        BRepBuilderAPI_Transform(
                            BRepBuilderAPI_Transform(
                                *one->shape,
                                xRotation
                            ).Shape(),
                            yRotation
                        ),
                        zRotation
                    );
                }
                case TRANSLATION: {
                    NodeShape* one = static_cast<NodeShape*>(args[0]);
                    NodeArray* two = static_cast<NodeArray*>(args[1]);


                    double x = static_cast<NodeNumber*>(two->array)->value;
                    double y = static_cast<NodeNumber*>(two->array->nextExpr)->value;
                    double z = static_cast<NodeNumber*>(two->array->nextExpr->nextExpr)->value;


                    gp_Trsf transformation;
                    transformation.SetTranslation(gp_Vec(x, y, z));

                   return new BRepBuilderAPI_Transform(*one->shape, transformation);
                }
            }
        }
    }
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
        case 0: {
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


    delete mkSeg;
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

    delete mkArc;
    return new BRepBuilderAPI_MakeEdge(aSegment);    
}


NodeArray* _validateLineTo(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        /* First array [point_0 ... point_N],
         * or incoming point [x, y, z]
         *
         * second is current point [x, y, z]
        */
        { {ARRAY, "POINT"}, {ARRAY, "POINT"} }, 

        /* First array [point_0 ... point_N],
         * or incoming point [x, y, z]
        */
        { {ARRAY, "POINT"}, {POINT, "POINT"} }, 

        { {POINT, "POINT"}, {POINT, "POINT"} }, 
        { {POINT, "POINT"}, {ARRAY, "POINT"} }, 
    };

    int argIndex = validateFunctionArguments(param, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "lineTo");
    }
    
    NodeArray* prevPoints = NULL;
    NodeArray* returnArray = NULL;     



    if((argIndex == 0 )|| (argIndex == 1)){
        //Figure out if we are [point_0, point_n] or [x, y, z]
        NodeArray* argsArray = static_cast<NodeArray*>(args[0]); 

        //If we are [point_0, point_n]
        if(checkAllExprTypes(argsArray->array, POINT)){
            returnArray =  argsArray;
        }
        else if(checkAllExprTypes(argsArray->array, DOUBLE)){
            if(getExpressionLength(argsArray->array) != 3){
                fprintf(stderr, "dest point array must be of length 3 ... exiting ...\n");
                exit(1);
            }

            std::vector<NodeExpression*> pointArr {
                argsArray
            };
            returnArray = newArrayNode(_makePoint(pointArr));
        }

        //If we made it this far we have our array of points from lhs
        if(args[1]->nodeType == ARRAY) {
            NodeArray* newArr = static_cast<NodeArray*>(args[1]);

            if(!checkAllExprTypes(newArr->array, DOUBLE)){
                fprintf(stderr, "rhs lineTo array all members do not evalutae to double ... exiting ...\n");
                exit(1);
            }
            if(getExpressionLength(newArr->array) != 3){
                fprintf(stderr, "rhs lineTo array must be length 3... exiting ...\n");
                exit(1);
            }
            
            std::vector<NodeExpression*> param { newArr };

            appendExprLinkedList(
                &returnArray->array,
                _makePoint(param)
            );

            return returnArray;
        }
        else if(args[1]->nodeType == POINT) {
            appendExprLinkedList(
                &returnArray->array,
                args[1]
            );
            return returnArray;
        }
        else {
            fprintf(stderr, "Cannot create line from lineTo ... exiting ... \n");
            exit(1);
        }
    }
    else if((argIndex == 2 )|| (argIndex == 3)){
        returnArray = newArrayNode(args[0]);


        if(args[1]->nodeType == POINT){
            appendExprLinkedList(
                &returnArray->array,
                args[1]
            );
            return returnArray;
        }
        else if(args[1]->nodeType == ARRAY){  
            std::vector<NodeExpression*> pointArr {args[1]}; 
            appendExprLinkedList(
                &returnArray->array,
                _makePoint(pointArr)
            );
            return returnArray;
        }
        else {
            fprintf(stderr, "Unable to create lineTo to the rhs point ... exiting ...\n");
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Unable to create lineTo ... extiting ... \n");
        exit(1);
    }
}


BRepBuilderAPI_MakeWire* _validateConnect(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {EDGE, "EDGE1"}, {EDGE, "EDGE2"} },
        { {EDGE, "EDGE1"}, {EDGE, "EDGE2"}, {EDGE, "EDGE3"} }
    };

    
    int argIndex = validateFunctionArguments(param, args);
    if(argIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "connect");
    }

    BRepBuilderAPI_MakeWire* myWire = new BRepBuilderAPI_MakeWire();

    for(int index = 0; index < param[argIndex].size(); index ++){
        NodeEdge* myEdge = static_cast<NodeEdge*>(args[index]);

        switch(myEdge->edgeType){
            case type_edge: {
                myWire->Add(*myEdge->edge);
                break;
            }
            case type_wire: {
                myWire->Add(*myEdge->wireShape);
                break;
            }
            case type_error: {
                fprintf(stderr, "Trying to connect a type_error wire\n"); 
                exit(1);
            }
        }
    }

    if(myWire->Error() != BRepBuilderAPI_WireDone){
        fprintf(stderr, "When connecting edges erorr occured %s ... exiting ... ", wireContructionError(myWire->Error()));
        exit(1);
    }

    return myWire; 
}


void _validateMirror(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {EDGE, "EDGE"} },
        { {SHAPE, "SHAPE"} }
    };

    
    int paramIndex = validateFunctionArguments(param, args);
    if(paramIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "mirror");
    }
}


BRepBuilderAPI_MakeFace* _validateFace(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {EDGE, "EDGE"} },
        { {ARRAY, "POINTARRAY"} }
    };

    int paramIndex = validateFunctionArguments(param, args);

    if(paramIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "makeFace");
    }
    
    switch(paramIndex){
        case 0: {
            NodeEdge* myEdge = static_cast<NodeEdge*>(args[0]);
            if(myEdge->brepEdge && myEdge->edge){
                TopoDS_Wire wire = TopoDS::Wire(*myEdge->edge);

                return new BRepBuilderAPI_MakeFace(wire);
            }
            else if(myEdge->brepWire && myEdge->wireShape){
                return new BRepBuilderAPI_MakeFace(*myEdge->wireShape);
            }

            fprintf(stderr, "Unable to create face from edges\n");
            exit(1);
        }
        case 1: {
            BRepBuilderAPI_MakeWire myWire = BRepBuilderAPI_MakeWire();
            NodeArray* myArray = static_cast<NodeArray*>(args[0]);

            int exprLength = getExpressionLength(myArray->array);
            if(exprLength < 2){
                fprintf(stderr, "Must have at least 2 points to create a face ... extiting ...\n");
                exit(1);
            }
            if(!checkAllExprTypes(myArray->array, POINT)){
                fprintf(stderr, "All array members must be of type point\n");
                exit(1);
            }

            NodeExpression* currPoint = myArray->array;
            while(currPoint->nextExpr){
                myWire.Add(
                    BRepBuilderAPI_MakeEdge(
                        *static_cast<NodePoint*>(currPoint)->point,
                        *static_cast<NodePoint*>(currPoint->nextExpr)->point
                    )
                );
                
                currPoint = currPoint->nextExpr;
            }


            return new BRepBuilderAPI_MakeFace(myWire.Wire());
        }
        default: {
            fprintf(stderr, "Unable to make face ... exiting ... \n");
            exit(1);
        }
    }
}

BRepPrimAPI_MakePrism* _validateExtrude(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {SHAPE, "shape"}, {ARRAY, "dir" } }
    };

    int paramIndex = validateFunctionArguments(param, args);

    if(paramIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "extrude");
    }

    NodeShape* myShape = static_cast<NodeShape*>(args[0]);
    NodeArray* myArray = static_cast<NodeArray*>(args[1]);

    if(myShape->shapeType != FACE){
        fprintf(stderr, "Extrution can only be performed on face ... exiting ... \n");
        exit(1);
    }
    if(getExpressionLength(myArray->array) != 3){
        fprintf(stderr, "Direction vector of extrude must be length 3 you passes %d ... exiting ...\n", getExpressionLength(myArray->array));
        exit(1);
    }
    if(!checkAllExprTypes(myArray->array, DOUBLE)){
        fprintf(stderr, "Direction vector of extrude must contain all double ... exiting ...\n");
        exit(1);
    }
    
    NodeNumber* num1 = static_cast<NodeNumber*>(myArray->array);
    NodeNumber* num2 = static_cast<NodeNumber*>(myArray->array->nextExpr);
    NodeNumber* num3 = static_cast<NodeNumber*>(myArray->array->nextExpr->nextExpr);

    gp_Vec vector(num1->value, num2->value, num3->value); 
    return new BRepPrimAPI_MakePrism(*myShape->shape, vector);
}


BRepFilletAPI_MakeFillet* _validateFillet(std::vector<NodeExpression*>& args, OCCT_SHAPE& shapeType)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {SHAPE, "shape"}, {DOUBLE, "radius" } }
    };

    int paramIndex = validateFunctionArguments(param, args);

    if(paramIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "fillet");
    }

    switch(paramIndex){
        case 0: {
            NodeShape* myShape = static_cast<NodeShape*>(args[0]);
            NodeNumber* r = static_cast<NodeNumber*>(args[1]);  

            BRepFilletAPI_MakeFillet* myFillet = new BRepFilletAPI_MakeFillet(*myShape->shape);

            for(TopExp_Explorer ex(*myShape->shape, TopAbs_EDGE); ex.More(); ex.Next()){
                myFillet->Add(r->value, TopoDS::Edge(ex.Current()));
            }
            
            shapeType = myShape->shapeType;
            return myFillet;
        }
        default: {

        }
    }
    
    fprintf(stderr, "Unable to validate fillet exiting ... \n");
    exit(1);
}


BRepFilletAPI_MakeChamfer* _validateChamfer(std::vector<NodeExpression*>& args, OCCT_SHAPE& shapeType)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {SHAPE, "shape"}, {DOUBLE, "dist" } }
    };

    int paramIndex = validateFunctionArguments(param, args);

    if(paramIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "fillet");
    }

    switch(paramIndex){
        case 0: {
            NodeShape* myShape = static_cast<NodeShape*>(args[0]);
            NodeNumber* d = static_cast<NodeNumber*>(args[1]);  

            BRepFilletAPI_MakeChamfer* myChamfer = new BRepFilletAPI_MakeChamfer(*myShape->shape);

            for(TopExp_Explorer ex(*myShape->shape, TopAbs_EDGE); ex.More(); ex.Next()){
                myChamfer->Add(d->value, TopoDS::Edge(ex.Current()));
            }
            
            shapeType = myShape->shapeType;
            return myChamfer;
        }
        default: {

        }
    }
    
    fprintf(stderr, "Unable to validate chamfer exiting ... \n");
    exit(1);
}


BRepPrimAPI_MakeTorus* _validateTorus(std::vector<NodeExpression*>& args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {DOUBLE, "r1"}, {DOUBLE, "r2"} },
        { {DOUBLE, "r1"}, {DOUBLE, "r2"}, {DOUBLE, "angle"} },
        { {DOUBLE, "r1"}, {DOUBLE, "r2"}, {DOUBLE, "a1"}, {DOUBLE, "a2"} },
        { {DOUBLE, "r1"}, {DOUBLE, "r2"}, {DOUBLE, "a1"}, {DOUBLE, "a2"}, {DOUBLE, "angle"} },
    };

    int paramIndex = validateFunctionArguments(param, args);
    
    if(paramIndex == -1){
        dumpArgumentsAndCorrectArguments(param, args, "makeTorus");
    }

    switch(paramIndex){
        case 0: {
            double r1 = static_cast<NodeNumber*>(args[0])->value;
            double r2 = static_cast<NodeNumber*>(args[1])->value;

            return new BRepPrimAPI_MakeTorus(r1, r2);
        }
        case 1: {
            double r1 = static_cast<NodeNumber*>(args[0])->value;
            double r2 = static_cast<NodeNumber*>(args[1])->value;
            double angle = static_cast<NodeNumber*>(args[2])->value;

            return new BRepPrimAPI_MakeTorus(r1, r2, angle);
        }
        case 2: {
            double r1 = static_cast<NodeNumber*>(args[0])->value;
            double r2 = static_cast<NodeNumber*>(args[1])->value;
            double a1 = static_cast<NodeNumber*>(args[2])->value;
            double a2 = static_cast<NodeNumber*>(args[3])->value;

            
            if( (0 < (a2 - a1)) && ( (a2 - a1) < 6.28 ) ){
                fprintf(stderr, "The angles a1, a2 must follow the relation 0 < a2 - a1 < 2*PI ... exiting ... \n");
                exit(1);
            }


            return new BRepPrimAPI_MakeTorus(r1, r2, a1, a2);
        }
        case 3: {
            double r1 = static_cast<NodeNumber*>(args[0])->value;
            double r2 = static_cast<NodeNumber*>(args[1])->value;
            double a1 = static_cast<NodeNumber*>(args[2])->value;
            double a2 = static_cast<NodeNumber*>(args[3])->value;
            double angle = static_cast<NodeNumber*>(args[4])->value;

            return new BRepPrimAPI_MakeTorus(r1, r2, a1, a2, angle);
        }
    }

    fprintf(stderr, "Unable to validate torus ... exiting ... \n");
    exit(1);
}


BRepPrimAPI_MakeRevol* _validateRevol(std::vector<NodeExpression*> & args)
{
    std::vector<std::vector<PARAM_INFO>> param {
        { {SHAPE, "S"}, {ARRAY, "POINT1"}, {ARRAY, "POINT2"}, {DOUBLE, "ANGLE"} },
        { {SHAPE, "S"}, {ARRAY, "POINT1"}, {ARRAY, "POINT2"} },
    };

    int paramIndex = validateFunctionArguments(param, args);
    
    if(paramIndex == 1){
        dumpArgumentsAndCorrectArguments(param, args, "Revol");
    }

    NodeShape* myShape = static_cast<NodeShape*>(args[0]);

    if(myShape->shapeType != FACE){
        fprintf(stderr, "Revolve can only be perfomed on a 2D shape"); 
        exit(1);
    }
    

    NodeArray* pointOneLocation = static_cast<NodeArray*>(args[1]);
    if(getExpressionLength(pointOneLocation->array) != 3){
        fprintf(stderr, "Point location array must be length 3\n") ;
        exit(1);
    }
    if(!checkAllExprTypes(pointOneLocation->array, DOUBLE)){
        fprintf(stderr, "Point location array must all be a double ... exiting ...\n");
        exit(1);
    }
    
    
    NodeArray* pointTwoLocation = static_cast<NodeArray*>(args[2]);
    if(getExpressionLength(pointTwoLocation->array) != 3){
        fprintf(stderr, "Direction array must be length 3\n") ;
        exit(1);
    }
    if(!checkAllExprTypes(pointTwoLocation->array, DOUBLE)){
        fprintf(stderr, "Direction array array must all be a double ... exiting ...\n");
        exit(1);
    }

    
    double point1X = static_cast<NodeNumber*>(pointOneLocation->array)->value;
    double point1Y = static_cast<NodeNumber*>(pointOneLocation->array->nextExpr)->value;
    double point1Z = static_cast<NodeNumber*>(pointOneLocation->array->nextExpr->nextExpr)->value;

    double point2X = static_cast<NodeNumber*>(pointTwoLocation->array)->value;
    double point2Y = static_cast<NodeNumber*>(pointTwoLocation->array->nextExpr)->value;
    double point2Z = static_cast<NodeNumber*>(pointTwoLocation->array->nextExpr->nextExpr)->value;

    gp_Pnt pointOne(point1X, point1Y, point1Z);
    gp_Pnt pointTwo(point2X, point2Y, point2Z);
    gp_Vec myVector(pointOne, pointTwo);


    switch(paramIndex){
        case 0: {
            NodeNumber* angle = static_cast<NodeNumber*>(args[3]);

            return new BRepPrimAPI_MakeRevol(*myShape->shape, gp_Ax1(pointOne, myVector), angle->value);
        }
        case 1: {
            return new BRepPrimAPI_MakeRevol(*myShape->shape, gp_Ax1(pointOne, myVector));
        }
        default: {
            fprintf(stderr, "Unable to validate revol ... exiting ...");
            exit(1);
        }
    }
}

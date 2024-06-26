#include "functions.hxx"
#include "gp_Pnt.hxx"
#include "node.hxx"
#include "symbolTable.hxx"
#include "enumToString.hxx"
#include "string.h"
#include <cstdio>
#include <cstdlib>
#include <string>

#define DEBUG_MODE 0
static int depth = 0;
static int insideFunction = 0;


void printText(int tabCount, const char* text)
{
    for(int i = 0; i < tabCount; i++){
        fprintf(stdout, "\t");
    }
    fprintf(stdout, "%s", text);
}


static NodeType* _createIdTypeFromNodeExpr(NODE_TYPE type)
{
    switch(type){
        case DOUBLE: {
            return newNodeType(num); 
        }
        case SHAPE: {
            return newNodeType(shape);
        }
        case POINT: {
            return newNodeType(point);
        }
        case EDGE: {
            return newNodeType(edge);
        }
        case FUNCTION: {
            return newNodeType(_void);
        }
        default: {
            return NULL;
        }
    }
}


static void _validateRhsEqualsDeclType(ID_TYPE lhs, nodeType rhs)
{
    if((rhs != DOUBLE) && (rhs != SHAPE) && (rhs != POINT) && (rhs != EDGE)){
        fprintf(stderr, "Right hand side of decl must either evaluate to a number, shape, point or edge ... exiting ...\n");
        exit(1);
    }

    switch(lhs){
        case num: {
            if(rhs != DOUBLE){
                fprintf(stderr, "You tried to assign %s, to a number\n", idTypeTostring(nodeTypeFromIdType(rhs)));
                exit(1);
            }
            return;
        }
        case shape: {
            if(rhs != SHAPE){
                fprintf(stderr, "You tried to assign %s, to a shape\n", idTypeTostring(nodeTypeFromIdType(rhs)));
                exit(1);
            }
            return;
        }
        case point: {
            if(rhs != POINT){
                fprintf(stderr, "You tried to assign %s, to a point\n", idTypeTostring(nodeTypeFromIdType(rhs)));
                exit(1);
            }
            return;
        }
        case edge: {
            if(rhs != EDGE){
                fprintf(stderr, "You tried to assign %s, to a edge\n", idTypeTostring(nodeTypeFromIdType(rhs)));
                exit(1);
            }
            return;
        }
        case _void: {
            fprintf(stderr, "You cannot assign to void\n");
            exit(1);
        }
    }

}



/*when we initilziz symTableHead*/
static SymbolTableHead* symTableHead = NULL;



/* Forward function declarations */

static NodeExpression* _processStmtListNode(NodeStmtList* node);
static NodeExpression* _processStmtNode(Node* node);
static NodeExpression* _processExprStmt(NodeExprStmt* node);
static NodeExpression* _processBlockNode(NodeBlock* node);
static NodeExpression* _processDeclNode(NodeDecl* node);
static NodeExpression* _processReturnStmt(NodeReturnStmt node);
static NodeExpression* _processFunctionNode(NodeFunction* node);


static NodeExpression* evalExpr(NodeExpression* state);
static NodeExpression* _processBinOp(NodeBinaryOperator* binOp);


void semantic(NodeStmtList* head)
{
    symTableHead = newSymbolTable();

    
    if(head->nodeType != STMT_LIST || !head){
        fprintf(stderr, "Head does not have type STMT_LIST or possible NULL\n"); 
        exit(1);
    }

    _processStmtListNode(head);

    freeSymbolTable(&symTableHead);
}


NodeExpression* _processStmtListNode(NodeStmtList* stmtList)
{
    int size = getStmtListSize(stmtList);
    for(int index = 0; index < size; index++){
        NodeStatement* curr = indexStmtList(stmtList, index);

        if(!curr){
            fprintf(stderr, "Inside processStmtListNode was unable to index stmt list node ... exiting ...\n");
            exit(1);
        }

        NodeExpression* result = _processStmtNode(curr);
        if(result && (result->nodeType == RETURN_EVAL)){
            NodeReturnEvaluated* eval = static_cast<NodeReturnEvaluated*>(result);
            return eval->result;
        }
    }
    return NULL;
}


NodeExpression* _processExprStmt(NodeExprStmt* node)
{
    return evalExpr(node->expr);
}


void _binOpReAssign(NodeIdentifier* node, NodeExpression* rhs)
{
    Symbol* mySymbol = getSymbolNode(symTableHead, node->idName);

    if(!mySymbol){
        fprintf(stderr, "Trying to assign to %s before decloration ... exiting ...\n", node->idName);
        exit(1);
    }
    else if (mySymbol->symbolType != variable){
        fprintf(stderr, "TODO YOU CAN ONLY UPDATE VARIABLES FOR NOW ... exiting ...\n");
        exit(1);
    }

    switch(mySymbol->mutState){
        case MUT: {
            NodeExpression* nodeExpr = evalExpr(rhs);
            _validateRhsEqualsDeclType(mySymbol->idType, nodeExpr->nodeType);
            updateSymbol(mySymbol, nodeExpr);
            break;
        }
        case NON_MUT: {
            fprintf(stderr, "Trying to re assign to %s which is a non-mutable varaible\n", node->idName);
            exit(1);
        }
    }
}


NodeExpression* _processDeclNode(NodeDecl* node)
{
    if(!node->value){
        fprintf(stderr, "_processDeclNode: TODO!\n");
        exit(1);
    }

    NodeExpression* newExprStmt = evalExpr(node->value);


    if(!newExprStmt){
        fprintf(stderr, "The value of node \"%s\" which is a decl was NULL ... exiting ...\n", node->id->idName);
        exit(1);
    }
    //If we are a function call assign it to whatever the the return value was
    else if(newExprStmt->nodeType == RETURN_EVAL){
        NodeReturnEvaluated* funcCall = static_cast<NodeReturnEvaluated*>(newExprStmt);


        _validateRhsEqualsDeclType(node->type->idType, funcCall->nodeType);


        node->value = funcCall->result;
    }
    else if(!node->type){
        /* If we made it in here it means our variable was declared without a type so assign it a type*/         
        NodeType* type = _createIdTypeFromNodeExpr(newExprStmt->nodeType);

        if(!type){
            fprintf(stderr, "Unable to create type for %s, which was not declared with a type, rhs type was %s\n",
                node->id->idName,
                nodeTypeToString(newExprStmt->nodeType)

            );
            exit(1);
        }

        node->type = type;
        _validateRhsEqualsDeclType(node->type->idType, newExprStmt->nodeType);
        node->value = newExprStmt;
    }
    else{
        _validateRhsEqualsDeclType(node->type->idType, newExprStmt->nodeType);
        node->value = newExprStmt;
    }


    insertSymbolFromNode(symTableHead, node);


    return NULL;
}

NodeExpression* _processBlockNode(NodeBlock* node)
{
    appendNewBasicBlock(&symTableHead);

    if(node->stms){
        return _processStmtListNode(node->stms);
    }

    freeTopBlock(&symTableHead);

    return NULL;
}

NodeExpression* _processFunctionNode(NodeFunction* node)
{
    node->blockNumber = symTableHead->blockNumber;
    insertSymbolFromNode(symTableHead, node);
    return NULL;
}

NodeExpression* _processReturnStmt(NodeReturnStmt* node)
{
    return newReturnEvaluated(evalExpr(node->returnExpr));
}

NodeExpression* _processStmtNode(Node* node)
{
    switch (node->nodeType) {
        case STMT_LIST: {
            return _processStmtListNode(static_cast<NodeStmtList*>(node));
        }
        case RETURN_STMT: {
            return _processReturnStmt(static_cast<NodeReturnStmt*>(node));
        }
        case EXPR_STMT: {
            return _processExprStmt(static_cast<NodeExprStmt*>(node));
        }
        case DECL: {
            return _processDeclNode(static_cast<NodeDecl*>(node));
        }
        case BLOCK: {
            return _processBlockNode(static_cast<NodeBlock*>(node));
        }
        case FUNCTION: {
            return _processFunctionNode(static_cast<NodeFunction*>(node));
        }
        default: {
            fprintf(stderr, "Hit non stmt node in _processStmtNode: %s", nodeTypeToString(node->nodeType)); 
            exit(1);
        }
    }

    //Make compiler happy
    return NULL;
}


NodeExpression* _processBinOp(NodeBinaryOperator* binOp)
{
    if(binOp->binaryOperatorType == OP_PIPE){
        NodeExpression* lhs = evalExpr(binOp->lhs);

        if(binOp->rhs->nodeType != FUNCTION_CALL){
            fprintf(stderr, "RHS of pipe operator is not a function call\n");
                exit(1);
        }
        NodeFunctionCall* funcCall = static_cast<NodeFunctionCall*>(binOp->rhs);

        int checkPipe = checkForPipeInput(funcCall->args);
        if(checkPipe == -1){
            fprintf(stderr, "You tried to pipe input to a function call that expects not to be piped ... exiting ...\n"); 
            exit(1);
        }
        else if(checkPipe == -2){
            fprintf(stderr, "function call expects more than on epipe ... exiting ... \n");
            exit(1);
        }

        replacePipeInput(&funcCall->args, lhs, checkPipe);
        return evalExpr(funcCall);
    }
    else if(binOp->binaryOperatorType == OP_REASSIGN){
        
        if(binOp->lhs->nodeType != ID){
            fprintf(stderr, "Trying to assign to a non ID node, lhs evaluated to %s\n", nodeTypeToString(binOp->lhs->nodeType)); 
            exit(0);
        }

        NodeIdentifier* id = static_cast<NodeIdentifier*>(binOp->lhs);
        _binOpReAssign(id, evalExpr(binOp->rhs));


        return NULL;
    }

    NodeExpression* lhs = evalExpr(binOp->lhs);
    NodeExpression* rhs = evalExpr(binOp->rhs);


    if(lhs->nodeType != rhs->nodeType){
        fprintf(stderr, "--EXITING--: lhs: \"%s\" != rhs: \"%s\"",
        nodeTypeToString(lhs->nodeType), nodeTypeToString(rhs->nodeType));
        exit(0);
    }
    else if((lhs->nodeType != DOUBLE) || (rhs->nodeType != DOUBLE)) {
        fprintf(stderr, "--UNDEFINED BINOP--: lhs: \"%s\" op: \"%s\" rhs: \"%s\".. exiting ...\n", 
            nodeTypeToString(lhs->nodeType),
            nodeOpToString(binOp->binaryOperatorType),
            nodeTypeToString(rhs->nodeType)
        );
        exit(0);
    }

                
    switch(binOp->binaryOperatorType){
        case OP_PLUS: {
            NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs);
            NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs);
            return newNumberNode(lhsNum->value + rhsNum->value);
        }
        case OP_SUB: {
            NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs);
            NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs);
            return newNumberNode(lhsNum->value - rhsNum->value);
        }
        case OP_DIV: {
            NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs);
            NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs);
            return newNumberNode(lhsNum->value / rhsNum->value);
        }
        case OP_MUL: {
            NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs);
            NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs);
            return newNumberNode(lhsNum->value * rhsNum->value);
        }
        case OP_MOD: {
            NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs);
            NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs);
            return newNumberNode((int)lhsNum->value % (int)rhsNum->value);
        }
        default: {
            fprintf(stderr, "case(binOp) in evalExpr hit defualt case\n");
            exit(0);
        }
    }
}



/*
 * This function is called whenever we want to evaluate 
 * what a variable is equal to.
*/
 
NodeExpression* _processId(NodeIdentifier* id)
{
    Symbol* sym = getSymbolNode(symTableHead, id->idName);

    if(!sym){
        fprintf(stderr, "Looked for |%s| in sym table was unable to find exiting...\n", id->idName);
        exit(0);
    }
            
    switch(sym->idType){
        case num: {
            return newNumberNode(sym->numVal->value);
        }
        case shape: {
            return sym->shape;
        }
        case point: {
            return sym->point;
        }
        case edge: {
            return sym->edge;
        }
        case _void: {
            //TODO
        }
    }
    return NULL;
}

NodeExpression* _processShape(NodeShape* node)
{
    return node;
}


NodeExpression* _processPoint(NodePoint* node)
{
    NodePoint* newPntNode = newNodePoint();
    newPntNode->point =  new gp_Pnt(
        node->point->X(),
        node->point->Y(),
        node->point->Z()
    );

    return newPntNode;
}


NodeExpression* _processLine(NodeEdge* node)
{
    return node;
}


NodeExpression* _processNumber(NodeNumber* numberNode)
{
    return newNumberNode(numberNode->value);
}


NodeArray* _processArray(NodeArray* arrayNode)
{
    NodeExpression* arr = arrayNode->array;
    NodeExpression* newArr = NULL;
    
    int arrLength = getExpressionLength(arr);
    for(int index = 0; index < arrLength; index++) {
        NodeExpression* expr = indexExprList(arr, index);
        appendExprLinkedList(&newArr, evalExpr(expr));
    }

    return newArrayNode(newArr);
}


NodeExpression* _processUnaryOp(NodeUnaryOperator* unNode)
{
    NodeExpression* expr = evalExpr(unNode->lhs);

    
    if(expr->nodeType != DOUBLE){
        fprintf(stderr, "Trying to perfrom unary operator on non number ... exiting ...\n");
        exit(1);
    }

    return newNumberNode(-static_cast<NodeNumber*>(expr)->value);
}


NodeExpression* _processFunctionCall(NodeFunctionCall* funcCallNode)
{
    functionPtr* funcPtr = lookUpFunc(funcCallNode->id->idName);
    if(!funcPtr){
        Symbol* sym = getSymbolNode(symTableHead, funcCallNode->id->idName);
        if(!sym){
            fprintf(stderr, "Unable to find function with name %s ... exiting ... \n", funcCallNode->id->idName);
            exit(1);
        }
        else if(sym->symbolType != function){
            fprintf(stderr, "Found symbol %s but was not function ... exiting ... \n", sym->name);
            exit(1);
        }

        int declLength = getDeclListSize(sym->function->arguments);
        int exprLength = getExpressionLength(funcCallNode->args);
        if(exprLength != declLength) { 
            fprintf(stderr, "Calling function '%s' with incorrect number of arguments function declared with %d arguments called with %d\n",
                    sym->name,
                    declLength,
                    exprLength
            );


            exit(1);
        }

        NodeExpression* evaluatedExprs = NULL;     
        
        for(int index = 0; index < exprLength; index++){
            NodeExpression* epxressionIndexed = indexExprList(funcCallNode->args, index);
            NodeExpression* evaluated = evalExpr(epxressionIndexed);

            NodeDecl* resultDecl = indexDeclList(sym->function->arguments, index);

            if(idTypeFromNodeType(evaluated->nodeType) != resultDecl->type->idType){
                fprintf(stderr, "function call %s passed %s in %d%s arugment ... expected %s ... exiting ..\n",
                        funcCallNode->id->idName, 
                        idTypeTostring(idTypeFromNodeType(evaluated->nodeType)),
                        index,
                        numToStrPlace(index),
                        idTypeTostring(resultDecl->type->idType)
                );
                exit(1);
            }

            appendExprLinkedList(&evaluatedExprs, evaluated);
        }
        
        int evalExprLength = getExpressionLength(evaluatedExprs);

        if(evalExprLength != exprLength){
            fprintf(stderr, "function call with %d arguments called with %d arguments but was only able to evaluate %d arguments exiting ...\n",
                declLength,
                exprLength,
                evalExprLength
            );
            exit(1);
        }

        SymbolTableHead* temp = symTableHead; 
        symTableHead = functionCallNewSymbolTable(temp, sym);


        for(int index = 0; index < exprLength; index++){
            NodeExpression* epxressionIndexed = indexExprList(evaluatedExprs, index);
            NodeDecl* declIndexed = indexDeclList(sym->function->arguments, index);
            declIndexed->value = epxressionIndexed;
            insertSymbolFromNode(symTableHead, declIndexed);
        }

        NodeExpression* returnNode = _processStmtNode(sym->function->block);
        

        for(int index = 0; index < exprLength; index++){
            NodeDecl* declIndexed = indexDeclList(sym->function->arguments, index);
            declIndexed->value = NULL;
        }
        
        freeFunctionCallSymbolTable(&symTableHead, sym);
        symTableHead = temp; 


        if(!returnNode){
            return returnNode;
        }
        else {
            if(sym->function->returnType->idType == _void){
                fprintf(stderr, "Function %s is of type void but got a return value of %s ... exiting ... \n",
                    sym->function->id->idName,
                    idTypeTostring(idTypeFromNodeType(returnNode->nodeType))
                );
                exit(1);
            }
            else if(idTypeFromNodeType(returnNode->nodeType) != sym->function->returnType->idType){
                fprintf(stderr, "Function %s returned %s, but its return type is %s ... exiting ... \n",
                    sym->function->id->idName,
                    idTypeTostring(idTypeFromNodeType(returnNode->nodeType)),
                    idTypeTostring(sym->function->returnType->idType)
                );
                exit(1);
            }
            return returnNode;
        }
    }
    else{
        //TODO: FOR NOW ONLY CALLING FUNCTIONS WITH ONE
        //      PARAM NEED TO HANDLE MULTIPLE
        std::vector<NodeExpression*> args;

        NodeExpression* currArg = funcCallNode->args;

        while(currArg){
            NodeExpression* next = currArg->nextExpr;
            args.push_back(evalExpr(currArg));
            currArg = next;
        }

        
        return execFunc(funcPtr, args);
    }

    //TODO
    return NULL;
}


NodeExpression* evalExpr(NodeExpression* state)
{ 
    switch (state->nodeType) {
        case BIN_OP: {
            return _processBinOp(static_cast<NodeBinaryOperator*>(state));
        }
        case ID: {
            return _processId(static_cast<NodeIdentifier*>(state));
        }
        case DOUBLE: {
            return _processNumber(static_cast<NodeNumber*>(state));
        }
        case FUNCTION_CALL: {
            return _processFunctionCall(static_cast<NodeFunctionCall*>(state));
        }
        case SHAPE: {
            return _processShape(static_cast<NodeShape*>(state));
        }
        case POINT: {
            return _processPoint(static_cast<NodePoint*>(state));
        }
        case EDGE: {
            return _processLine(static_cast<NodeEdge*>(state));
        }
        case ARRAY: {
            return _processArray(static_cast<NodeArray*>(state));
        }
        case UN_OP: {
            return _processUnaryOp(static_cast<NodeUnaryOperator*>(state));
        }
        default: {
            fprintf(stderr, "evalExpr/semantic.cpp invalid nodeType: %s\n", nodeTypeToString(state->nodeType));
            exit(0);
        }
    }

    fprintf(stderr, "ERROR OUT OF SWITCH STATEMENT exiting...\n");
    exit(0);
}


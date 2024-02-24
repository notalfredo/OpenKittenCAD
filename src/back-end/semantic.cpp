#include "functions.hxx"
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


void quitMessage(const char* msg)
{
    fprintf(stderr, "=============================\n");
    fprintf(stderr, "%s exiting...", msg);
    fprintf(stderr, "=============================\n");
}


/*when we initilziz symTableHead*/
static SymbolTableHead* symTableHead = NULL;



/* Forward function declarations */
static NodeExpression* evalExpr(NodeExpression* state);

static void _processStmtListNode(NodeStmtList* node);
static void _processStmtNode(Node* node);
static void _processExprStmt(NodeExprStmt* node);
static void _processBlockNode(NodeBlock* node);
static void _processDeclNode(NodeDecl* node);
static NodeExpression* _processReturnStmt(NodeReturnStmt node);


static NodeExpression* _processBinOp(NodeBinaryOperator* binOp);


void semantic(NodeStmtList* head)
{
    symTableHead = newSymbolTable();
    
    if(head->nodeType != STMT_LIST || !head){
        quitMessage("Head does not have type STMT_LIST or possible NULL\n");
        exit(1);
    }

    _processStmtListNode(head);

    freeSymbolTable(&symTableHead);
}


void _processStmtListNode(NodeStmtList* stmtList)
{
    int size = getStmtListSize(stmtList);
    for(int index = 0; index < size; index++){
        NodeStatement* curr = indexStmtList(stmtList, index);

        if(!curr){
            fprintf(stderr, "Inside processStmtListNode was unable to index stmt list node ... exiting ...\n");
            exit(1);
        }

        _processStmtNode(curr);
    }
}


void _processExprStmt(NodeExprStmt* node)
{
    evalExpr(node->expr);
}


void _processDeclNode(NodeDecl* node)
{
    if(!node->value){
        quitMessage("_processDeclNode: TODO!\n");
    }

    NodeExpression* newExprStmt = evalExpr(node->value);
    node->value = newExprStmt;
    insertSymbolFromNode(symTableHead, node);

    //dumpSymbolTable(symTableHead);
}

void _processBlockNode(NodeBlock* node)
{
    appendNewBasicBlock(&symTableHead);

    if(node->stms){
        _processStmtListNode(node->stms);
    }

    freeTopBlock(&symTableHead);
}

void _processFunctionNode(NodeFunction* node)
{
    node->blockNumber = symTableHead->blockNumber;
    insertSymbolFromNode(symTableHead, node);
}

NodeExpression* _processReturnStmt(NodeReturnStmt* node)
{
    return evalExpr(node->returnExpr);
}

void _processStmtNode(Node* node)
{
    switch (node->nodeType) {
        case STMT_LIST: {
            _processStmtListNode(static_cast<NodeStmtList*>(node));
            return;
        }
        case RETURN_STMT: {
            _processReturnStmt(static_cast<NodeReturnStmt*>(node));
            return;
        }
        case EXPR_STMT: {
            _processExprStmt(static_cast<NodeExprStmt*>(node));
            return;
        }
        case DECL: {
            _processDeclNode(static_cast<NodeDecl*>(node));
            return;
        }
        case BLOCK: {
            _processBlockNode(static_cast<NodeBlock*>(node));
            return;
        }
        case FUNCTION: {
            _processFunctionNode(static_cast<NodeFunction*>(node));
            return;
        }
        default: {
            std::string msg = "Hit non stmt node in _processStmtNode: " + std::string(nodeTypeToString(node->nodeType));
            quitMessage(msg.c_str());
        }
    }
}


NodeExpression* _processBinOp(NodeBinaryOperator* binOp)
{
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
        case OP_ASSIGN: {
            if(lhs->nodeType != ID){
                fprintf(stderr, "Trying to assign to a non ID node\n"); 
                exit(0);
            }
            //TODO UPDATE LHS IN SYMBOL TABLE
        }
        default: {
            fprintf(stderr, "case(binOp) in evalExpr hit defualt case\n");
                exit(0);
            }
        }
}


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
            //TODO
        }
        case _void: {
            //TODO
        }
    }
    return NULL;
}


NodeExpression* _processNumber(NodeNumber* numberNode)
{
    return numberNode;
}


NodeExpression* _processFunctionCall(NodeFunctionCall* funcCallNode)
{
    functionPtr* funcPtr = lookUpFunc(funcCallNode->id->idName);
    if(!funcPtr){
        Symbol* sym = getSymbolNode(symTableHead, funcCallNode->id->idName);
        if(!sym){
            fprintf(stderr, "Unable to find function with name %s ... exiting ... \n", sym->name);
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

        _processStmtNode(sym->function->block);
        

        for(int index = 0; index < exprLength; index++){
            NodeDecl* declIndexed = indexDeclList(sym->function->arguments, index);
            declIndexed->value = NULL;
        }
        
        freeFunctionCallSymbolTable(&symTableHead, sym);
        symTableHead = temp; 
    }
    else{
        //TODO: FOR NOW ONLY CALLING FUNCTIONS WITH ONE
        //      PARAM NEED TO HANDLE MULTIPLE
        execFunc(funcPtr, evalExpr(funcCallNode->args));
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
        default: {
            fprintf(stderr, "evalExpr/semantic.cpp invalid nodeType: %s\n", nodeTypeToString(state->nodeType));
            exit(0);
        }
    }

    fprintf(stderr, "ERROR OUT OF SWITCH STATEMENT exiting...\n");
    exit(0);
}

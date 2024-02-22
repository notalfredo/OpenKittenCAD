#include "functions.hxx"
#include "node.hxx"
#include "symbolTable.hxx"
#include "enumToString.hxx"
#include "string.h"
#include <cstdio>
#include <cstdlib>
#include <string>

#define DEBUG_MODE 1
static int depth = 0;


void printText(int tabCount, const char* text)
{
    for(int i = 0; i < tabCount; i++){
        fprintf(stdout, "\t");
    }
    fprintf(stdout, "%s", text);
}

void debugStmtNode(NODE_TYPE type, int enteringNode)
{
    
    if(DEBUG_MODE){
        if(!enteringNode){
            printText(depth, ")\n" );    
            return;
        }

        switch(type){
            case STMT_LIST: {
                printText(depth, "StmtListNode(\n");    
                return;
            }
            case EXPR_STMT: {
                printText(depth, "ExprNode(\n");
                return;
            }
            case DECL: {
                printText(depth, "DeclNode(\n");    
                return;
            }
            case BLOCK: {
                printText(depth, "BlockNode(\n");    
                return;
            }
            default: {
                
            }
        }
    }
}

void debugExprNode(NodeExpression* exprNode, int enteringNode)
{
    
    if(DEBUG_MODE){
        if(!enteringNode){
            printText(depth, ")\n" );    
            return;
        }

        switch(exprNode->nodeType){
            case BIN_OP: {
                printText(depth, "BinOpNode(\n");    
                return;
            }
            case ID: {
                printText(depth, "IdentifierNode(\n");
                return;
            }
            case DOUBLE: {
                printText(depth, "DoubleNode(\n");    
                return;
            }
            case FUNCTION_CALL: {
                printText(depth, "FunctionCallNode(\n");    
                return;
            }
            default: {
                
            }
        }
    }
}


void quitMessage(const char* msg)
{
    fprintf(stderr, "=============================\n");
    fprintf(stderr, "%s exiting...", msg);
    fprintf(stderr, "=============================\n");
}


/*when we initilziz symTableHead*/
static SymbolTableHead* symTableHead = NULL;

extern int getStmtListSize(NodeStmtList* list);
extern NodeStatement* indexStmtList(NodeStmtList* list, int index);


/* Forward function declarations */
static NodeExpression* evalExpr(NodeExpression* state);

static void _processStmtListNode(NodeStmtList* node);
static void _processStmtNode(Node* node);
static void _processExprStmt(NodeExprStmt* node);
static void _pocessBlockNode(NodeBlock* node);
static void _processDeclNode(NodeDecl* node);


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
        if(curr){
            _processStmtNode(curr);
        }
        else{
            //TODO HANDLE THIS
        }
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

void _pocessBlockNode(NodeBlock* node)
{
    appendNewBasicBlock(&symTableHead);

    if(node->stms){
        _processStmtListNode(node->stms);
    }

    freeTopBlock(&symTableHead);
}


void _processStmtNode(Node* node)
{
    switch (node->nodeType) {
        case STMT_LIST: {
            debugStmtNode(node->nodeType, 1);
            depth++;

            _processStmtListNode(static_cast<NodeStmtList*>(node));

            depth--;
            debugStmtNode(node->nodeType, 0);
            return;
        }
        case EXPR_STMT: {
            debugStmtNode(node->nodeType, 1);
            depth++;

            _processExprStmt(static_cast<NodeExprStmt*>(node));

            depth--;
            debugStmtNode(node->nodeType, 0);
            return;
        }
        case DECL: {
            debugStmtNode(node->nodeType, 1);
            depth++;

            _processDeclNode(static_cast<NodeDecl*>(node));

            depth--;
            debugStmtNode(node->nodeType, 0);
            return;
        }
        case BLOCK: {
            debugStmtNode(node->nodeType, 1);
            depth++;

            _pocessBlockNode(static_cast<NodeBlock*>(node));

            depth--;
            debugStmtNode(node->nodeType, 0);
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
            return sym->numVal;
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
        //TODO LOOK UP DECLARED FUNCTION FOR NOW JUST EXIT
        fprintf(stderr, "UNABLE TO FIND FUNCTION EXITING...\n");
        exit(0);
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
            debugExprNode(state, 1);
            depth++;

            NodeExpression* result = _processBinOp(static_cast<NodeBinaryOperator*>(state));
            depth--;
            debugExprNode(state, 0);
            return result;
        }
        case ID: {
            debugExprNode(state, 1);
            depth++;
            
            NodeExpression* result = _processId(static_cast<NodeIdentifier*>(state));

            depth--;
            debugExprNode(state, 0);
            return result;
        }
        case DOUBLE: {
            debugExprNode(state, 1);
            depth++;
            
            NodeExpression* result = _processNumber(static_cast<NodeNumber*>(state));

            depth--;
            debugExprNode(state, 0);
            return result;
        }
        case FUNCTION_CALL: {
            debugExprNode(state, 1);
            depth++;

            NodeExpression* result = _processFunctionCall(static_cast<NodeFunctionCall*>(state));
            depth--;
            debugExprNode(state, 0);
            return result;
        }
        default: {
            fprintf(stderr, "evalExpr/semantic.cpp invalid nodeType: %s\n", nodeTypeToString(state->nodeType));
            exit(0);
        }
    }

    fprintf(stderr, "ERROR OUT OF SWITCH STATEMENT exiting...\n");
    exit(0);
}

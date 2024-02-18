#include "node.hxx"
#include "symbolTable.hxx"
#include "enumToString.hxx"
#include <cstdio>


/*when we initilziz symTableHead*/
static SymbolTableHead* symTableHead = newSymbolTable();

extern int getStmtListSize(NodeStmtList* list);
NodeStatement* indexStmtList(NodeStmtList* list, int index);


NodeExpression* evalExpr(NodeExpression* state)
{
    switch(state->nodeType){
        switch (state->nodeType) {
            case BIN_OP: {
                NodeBinaryOperator* binOp = static_cast<NodeBinaryOperator*>(state);
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
                        NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs->nextExpr);
                        NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs->nextExpr);
                        return newNumberNode(lhsNum->value + rhsNum->value);
                    }
                    case OP_SUB: {
                        NodeNumber* lhsNum = static_cast<NodeNumber*>(lhs->nextExpr);
                        NodeNumber* rhsNum = static_cast<NodeNumber*>(rhs->nextExpr);
                        return newNumberNode(lhsNum->value - rhsNum->value);
                    }
                    default: {
                        fprintf(stderr, "case(binOp) in evalExpr hit defualt case\n");
                        exit(0);
                    }
                }
        

                break;
            }
            case DOUBLE: {
                break;
            }
            case ID: {
                //TODO
                break;
            }
            case FUNCTION_CALL: {
                //TODO
                break;
            }
            default: {
                fprintf(stderr, "evalExpr/semantic.cpp invalid nodeType: %s\n", nodeTypeToString(state->nodeType));
                exit(0);
            }
        }
    }


    NodeBinaryOperator* bop = static_cast<NodeBinaryOperator*>(state);
    

}

void semantic(Node* state)
{
    switch (state->nodeType){
        case STMT_LIST: {
            NodeStmtList* stmtList = static_cast<NodeStmtList*>(state);
            int size = getStmtListSize(stmtList);

            for(int index = 0; index < size; index++){
                NodeStatement* curr = indexStmtList(stmtList, index);
                if(curr){
                    semantic(curr);
                }
                else{
                    //TODO HANDLE THIS
                }
            }
            break;
        }
        case BLOCK: {
            NodeBlock* block = static_cast<NodeBlock*>(state);
            appendNewBasicBlock(&symTableHead);
            semantic(block->stms);
            freeTopBlock(&symTableHead);
            break;
        }
        case DECL: {
            NodeDecl* decl = static_cast<NodeDecl*>(state);
            NodeExpression* newExprStmt = evalExpr(decl->value->expr);
            decl->value->expr = newExprStmt;

            insertSymbolFromNode(symTableHead, decl);


            break;
        }
        case EXPR_STMT: {
            //NodeExprStmt* exprStmt = static_cast<NodeExprStmt*>(state);
            //evalExpr(exprStmt->expr);
            break;
        }
        default: {
            fprintf(stderr, "Hit default case semantic.cpp\
                            semantic %d\n", state->nodeType);
            break;
        }
    }
}

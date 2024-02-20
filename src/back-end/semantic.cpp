#include "functions.hxx"
#include "node.hxx"
#include "symbolTable.hxx"
#include "enumToString.hxx"
#include <cstdio>
#include <cstdlib>


/*when we initilziz symTableHead*/
static SymbolTableHead* symTableHead = newSymbolTable();

extern int getStmtListSize(NodeStmtList* list);
NodeStatement* indexStmtList(NodeStmtList* list, int index);


NodeExpression* evalExpr(NodeExpression* state)
{ switch (state->nodeType) {
        case BIN_OP: {
            fprintf(stderr, "Entering BIN_OP\n");
            NodeBinaryOperator* binOp = static_cast<NodeBinaryOperator*>(state);
            NodeExpression* lhs = evalExpr(binOp->lhs);
            NodeExpression* rhs = evalExpr(binOp->rhs);
                


            fprintf(stderr, "FIRST\n");
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
            fprintf(stderr, "SECOND\n");
                
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
            fprintf(stderr, "Leaving BIN_OP\n");
            break;
        }
        case ID: {
            NodeIdentifier* idNode = static_cast<NodeIdentifier*>(state);

            Symbol* sym = getSymbolNode(symTableHead, idNode->idName);

            if(!sym){
                fprintf(stderr, "Looked for |%s| in sym table was unable to find exiting...\n", idNode->idName);
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
        }
        case DOUBLE: {
            fprintf(stderr, "Entering DOUBLE\n");
            return state;
        }
        case FUNCTION_CALL: {
            fprintf(stderr, "Entering FUNCTION CALL\n");
            NodeFunctionCall* funcCallNode = static_cast<NodeFunctionCall*>(state);
            functionPtr* funcPtr = lookUpFunc(funcCallNode->id->idName);
            fprintf(stderr, "===================\n");
            if(!funcPtr){
                //TODO LOOK UP DECLARED FUNCTION FOR NOW JUST EXIT
                fprintf(stderr, "UNABLE TO FIND FUNCTION EXITING...\n");
                exit(0);
            }
            else{
                fprintf(stderr, "===================\n");
                //TODO: FOR NOW ONLY CALLING FUNCTIONS WITH ONE
                //      PARAM NEED TO HANDLE MULTIPLE
                execFunc(funcPtr, evalExpr(funcCallNode->args));
            }


            //TODO
            return NULL;
        }
        default: {
            fprintf(stderr, "evalExpr/semantic.cpp invalid nodeType: %s\n", nodeTypeToString(state->nodeType));
            exit(0);
        }
    }

    fprintf(stderr, "ERROR OUT OF SWITCH STATEMENT exiting...\n");
    exit(0);
}


void semFreeSymbolTable()
{
    freeSymbolTable(&symTableHead);
}


void semantic(Node* state)
{
    switch (state->nodeType){
        case STMT_LIST: {
            fprintf(stderr, "Entering STMT_LIST\n");
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
            fprintf(stderr, "Leaving STMT_LIST\n");
            break;
        }
        case BLOCK: {
            fprintf(stderr, "Entering BLOCK\n");
            NodeBlock* block = static_cast<NodeBlock*>(state);
            appendNewBasicBlock(&symTableHead);
            semantic(block->stms);
            freeTopBlock(&symTableHead);
            fprintf(stderr, "Leaving BLOCK\n");
            break;
        }
        case DECL: {
            fprintf(stderr, "Entering DECL\n");
            NodeDecl* decl = static_cast<NodeDecl*>(state);
            fprintf(stderr, "==========\n");
            if(!decl->value){
                fprintf(stderr, "***\n");
            }

            NodeExpression* newExprStmt = evalExpr(decl->value);
            fprintf(stderr, "==========\n");
            decl->value = newExprStmt;

            fprintf(stderr, "==========\n");
            insertSymbolFromNode(symTableHead, decl);

            dumpSymbolTable(symTableHead);

            
            fprintf(stderr, "Leaving DECL\n");
            break;
        }
        case EXPR_STMT: {
            fprintf(stderr, "Entering EXPR_STMT\n");
            NodeExprStmt* exprStmt = static_cast<NodeExprStmt*>(state);
            evalExpr(exprStmt->expr);
            fprintf(stderr, "Leaving EXPR_STMT\n");
            break;
        }
        default: {
            fprintf(stderr, "Hit default case semantic.cpp\
                            semantic %d\n", state->nodeType);
            break;
        }
    }
}

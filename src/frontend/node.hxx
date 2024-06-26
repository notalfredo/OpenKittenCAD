#ifndef NODE_H
#define NODE_H

#include "BRepBuilderAPI_MakeShape.hxx"
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

#include "TopoDS_Edge.hxx"

#include "gp_Pnt.hxx"
#include <TopoDS_Shape.hxx>

typedef enum declMutState {
    MUT,
    NON_MUT,
} DECL_MUT_STATE;


typedef enum edgeType {
    type_edge,
    type_wire,
    type_error
} EDGE_TYPE;


typedef enum dim {
    TWO_DIM,
    THREE_DIM,
} DIMENSION;

typedef enum nodeOp {
    OP_PLUS, 
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_REASSIGN,
    OP_PIPE,
    OP_NEGATE
} NODE_OP;

typedef enum shape {
    BOX,
    CONE,
    CYLINDER,
    SPHERE,
    TORUS,
    FACE,
    CUSTOM
} OCCT_SHAPE;


typedef enum idType {
    num,
    shape,
    point,
    edge,
    _void, //only used for functions
} ID_TYPE;

typedef enum nodeType {
    IF,
    ELIF,
    ELSE,

    FOR,
    WHILE,
    REPEAT,
    UNTIL,

    DECL,
    STMT,

    FUNCTION,

    ID,
    FUNCTION_CALL,
    TYPE,

    BLOCK,

    ASSIGN,

    BIN_OP,
    UN_OP,

    DOUBLE,
    SHAPE,
    POINT,
    EDGE,

    RETURN_EVAL,
    PLACEHOLDER,

    ARRAY,

    STMT_LIST,
    DECL_LIST,
    EXPR_STMT,
    RETURN_STMT,
} NODE_TYPE;



#include <string>
#include <vector>


class Node {
    public:
        virtual ~Node() {}

        /*
         We use nodeType when we are doing semantic analysis
         to determine the type of node we are visiting. We 
         do this since there is no "nice" way to do it in c++
        */
        NODE_TYPE nodeType; 
        Node* _allocatedLinkedList;
};


class NodeStatement: public Node {
    public:
        NodeStatement* nextStmt; 
};
class NodeExpression: public Node {
    public:
        NodeExpression* nextExpr;
};


class NodeType: public Node {
    public:
        ID_TYPE idType;
        NodeType(ID_TYPE idType, Node* _prevAlloc): idType(idType) {
            this->nodeType = TYPE;

            this->_allocatedLinkedList = _prevAlloc;
        }
};

/* 
   =======================================================
                     NODE EXPRESSIONS
   =======================================================
*/  
class NodeIdentifier: public NodeExpression {
    public:
        char* idName;
        NodeIdentifier(
            char* idName,
            Node* _prevAlloc
        ): idName(idName) {
            this->nodeType = ID;
            this->_allocatedLinkedList = _prevAlloc;
            this->nextExpr = NULL;
        }
};


class NodeFunctionCall: public NodeExpression {
    public:
        NodeIdentifier* id;
        NodeExpression* args;
        NodeExpression* returnedValue;
        NodeFunctionCall(
            NodeIdentifier* id,
            NodeExpression* args,
            Node* _prevAlloc
        ): id(id), args(args) {
            this->nodeType = FUNCTION_CALL,
            this->returnedValue = NULL;
            this->_allocatedLinkedList = _prevAlloc;
            this->nextExpr = NULL;
        }
};


class NodeNumber: public NodeExpression {
    public:
        double value; 
        NodeNumber(double value, Node* _prevAlloc): value(value) {
            this->nodeType = DOUBLE;
            this->_allocatedLinkedList = _prevAlloc;
            this->nextExpr = NULL;
        }; 
};


class NodeArray: public NodeExpression {
    public:
        NodeExpression* array;
        NodeArray(NodeExpression* array, Node* _prevAlloc): array(array) {
            this->nodeType = ARRAY; 
            this->_allocatedLinkedList = _prevAlloc;
            this->nextExpr = NULL;
        }
};

class NodePoint: public NodeExpression {
    public:
        gp_Pnt* point;
        NodePoint(Node* _prevAlloc) {
            this->point = NULL;
            this->nodeType = POINT;
            this->nextExpr = NULL;
            this->_allocatedLinkedList = _prevAlloc;
        }
};


class NodeEdge: public NodeExpression {
    public:
        BRepBuilderAPI_MakeEdge* brepEdge;    
        const TopoDS_Edge* edge;

        BRepBuilderAPI_MakeWire* brepWire; 
        const TopoDS_Wire* wireShape;
        
        EDGE_TYPE edgeType;

        NodeEdge(Node* _prevAlloc) {
            this->nodeType = EDGE;
            this->nextExpr = NULL;
            this->_allocatedLinkedList = _prevAlloc;

            this->brepEdge = NULL;
            this->edge = NULL;
            this->brepWire = NULL;
            this->wireShape = NULL;
            this->edgeType = type_error;
        }
};


class NodeShape: public NodeExpression {
    public:
        OCCT_SHAPE shapeType;
        BRepBuilderAPI_MakeShape* brepShape; 
        const TopoDS_Shape* shape;


        NodeShape(OCCT_SHAPE shapeType, Node* _prevAlloc): shapeType(shapeType){
            this->nodeType = SHAPE;
            this->_allocatedLinkedList = _prevAlloc;
            this->nextExpr = NULL;

            this->shape = NULL;
            this->brepShape = NULL;
        }
};

class NodeUnaryOperator: public NodeExpression {
    public:
        NodeExpression* lhs;
        NODE_OP unaryOperatorType;
        NodeUnaryOperator(NodeExpression* lhs, NODE_OP unaryOperatorType, Node* _prevAlloc): lhs(lhs), unaryOperatorType(unaryOperatorType) {
            _allocatedLinkedList = _prevAlloc; 
            this->nextExpr = NULL;
            this->nodeType = UN_OP;
        }
        
};

class NodeBinaryOperator: public NodeExpression {
    public:
        NodeExpression* lhs;
        NodeExpression* rhs;
        NODE_OP binaryOperatorType;
        NodeBinaryOperator(
               NodeExpression* lhs,
               NodeExpression* rhs,
               NODE_OP binaryOperatorType,
               Node* _prevAlloc
        ): lhs(lhs), rhs(rhs), binaryOperatorType(binaryOperatorType) {
            this->nodeType =  BIN_OP;
            this->_allocatedLinkedList = _prevAlloc;
            this->nextExpr = NULL;
        }
};


class NodeReturnEvaluated: public NodeExpression {
    public:
        NodeExpression* result;
        NodeReturnEvaluated(NodeExpression* result, Node* _prevAlloc): result(result){
            this->_allocatedLinkedList = _prevAlloc;
            this->nodeType = RETURN_EVAL;
            this->nextExpr = NULL;
        }
};


class NodePlaceHolder: public NodeExpression {
    public:     
        NodePlaceHolder(Node* _prevAlloc){
            this->_allocatedLinkedList = _prevAlloc;
            this->nodeType = PLACEHOLDER;
            this->nextExpr = NULL;
        }
};


/* 
   =======================================================
                     NODE STATEMENTS  
   =======================================================
*/  
class NodeReturnStmt: public NodeStatement {
    public:
        NodeExpression* returnExpr;
        NodeReturnStmt(NodeExpression* returnExpr, Node* _prevAlloc): returnExpr(returnExpr) {
        this->nodeType = RETURN_STMT;
        this->_allocatedLinkedList = _prevAlloc;
        this->nextStmt = NULL;
    }
};


class NodeExprStmt: public NodeStatement {
    public:
        NodeExpression* expr;
        NodeExprStmt(NodeExpression* expr, Node* _prevAlloc):expr(expr){
            this->nextStmt = NULL;
            this->nodeType = EXPR_STMT;
            this->_allocatedLinkedList = _prevAlloc;
        }
};


class NodeDecl: public NodeStatement {
    public:
        NodeIdentifier* id;
        NodeType* type;
        NodeDecl* nextDecl;
        NodeExpression* value;
        DECL_MUT_STATE mutState; 

        NodeDecl(
            NodeIdentifier* id,
            NodeType* type,
            NodeExpression* value,
            DECL_MUT_STATE mutState,
            Node* _prevAlloc
        ): id(id), type(type), value(value), mutState(mutState) {
            this->nextStmt = NULL;
            this->nextDecl = NULL;
            this->nodeType = DECL;

            this->_allocatedLinkedList = _prevAlloc;
        };
};


class NodeStmtList: public Node {
    public:
        NodeStatement* nextStmt; 
        NodeStmtList(NodeStatement* nextStmt, Node* _prevAlloc)
            : nextStmt(nextStmt) {
            this->nodeType = STMT_LIST;

            this->_allocatedLinkedList = _prevAlloc;
        }
};

class NodeDeclList: public Node {
    public:
        NodeDecl* nextDecl;
        NodeDeclList(NodeDecl* nextDecl, Node* _prevAlloc)
            : nextDecl(nextDecl) {
            this->nodeType = DECL_LIST;

            this->_allocatedLinkedList = _prevAlloc;
        }
};

class NodeBlock: public NodeStatement {
    public:
        NodeStmtList* stms;
        NodeBlock(
            NodeStmtList* stms,
            Node* _prevAlloc
        ): stms(stms) {
            this->nextStmt = NULL;
            this->nodeType = BLOCK;

            this->_allocatedLinkedList = _prevAlloc;
        }
};


class NodeFunction: public NodeStatement {
    public: 

        int blockNumber;
        NodeIdentifier* id;
        NodeDeclList* arguments;
        NodeType*  returnType;
        NodeBlock* block;
        NodeFunction(
            NodeIdentifier* id,
            NodeDeclList* arguments,
            NodeType*  returnType,
            NodeBlock* block,
            Node* _prevAlloc
        ): id(id), arguments(arguments), returnType(returnType), block(block) {
            this->nextStmt = NULL;
            this->nodeType = FUNCTION;
            this->_allocatedLinkedList = _prevAlloc;
            this->blockNumber = 0;
        }
};

class NodeBinaryAssign: public NodeStatement {
    public:
        NodeIdentifier* id;
        NodeExpression* assignment;
        NodeBinaryAssign(NodeIdentifier* id,
                         NodeExpression* assignment
        ): id(id), assignment(assignment){
            this->nextStmt = NULL;
            this->nodeType = ASSIGN;
        }
};




class NodeRepeat: public NodeStatement {
    public:
        NodeBlock* block;
        NodeRepeat(NodeBlock* block): block(block) {
            this->nextStmt = NULL;
            this->nodeType = REPEAT;
        };
};


class NodeUntil: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeUntil(
            NodeExpression* condition,
            NodeBlock* block
        ): condition(condition), block(block)  {
            this->nextStmt = NULL;
            this->nodeType = UNTIL;
        };
};


class NodeWhile: public NodeStatement {
    public:
        NodeExpression* condition; 
        NodeBlock* block;
        NodeWhile(
            NodeExpression* condition,
            NodeBlock* block
        ): condition(condition), block(block)  {
            this->nextStmt = NULL;
            this->nodeType = WHILE;
        };

};


class NodeFor: public NodeStatement {
    public:
        NodeIdentifier* localVal; 
        NodeExpression* condition;
        NodeExpression* update;
        NodeFor(
            NodeIdentifier* localVal,
            NodeExpression* condition,
            NodeExpression* update
        ): localVal(localVal), condition(condition), update(update)  {
            this->nextStmt = NULL;
            this->nodeType = FOR;
        };

};
 

class NodeElse: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeElse(NodeExpression* condition): condition(condition) {
            this->nextStmt = NULL;
            this->nodeType = ELSE;
        };
};


class NodeElif: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeElif(NodeExpression* condition, NodeBlock* block): 
            condition(condition), block(block) {
            this->nextStmt = NULL;
            this->nodeType = ELIF;
        };
};


class NodeIf: public NodeStatement {
    public:
        NodeExpression* condition;
        std::vector<NodeElif*> elif;
        NodeBlock* block;
        NodeIf(NodeExpression* condition, NodeBlock* block):
            condition(condition), block(block) {
            this->nextStmt = NULL;
            this->nodeType = IF;
        };
};


// nodeAllocFree.cpp
NodeType* newNodeType(ID_TYPE idType);
NodeIdentifier* newIdentifierNode(char* idName);
NodeNumber* newNumberNode(double value);
NodeShape* newNodeShape(OCCT_SHAPE shape);
NodeEdge* newNodeEdge();
NodePoint* newNodePoint();
NodeBinaryOperator* newBinaryOperatorNode(NodeExpression* lhs, NodeExpression* rhs, NODE_OP binaryOperatorType);
NodeDecl* newDeclNode(NodeIdentifier* id, NodeType* type, NodeExpression* value, DECL_MUT_STATE mutState);
NodeStmtList* newStmtList(NodeStatement* nextStmt);
NodeDeclList* newDeclList(NodeDecl* nextDecl);
NodeBlock* newNodeBlock(NodeStmtList* stmts);
NodeFunction* newFunctionNode(NodeIdentifier* id, NodeDeclList* arguments, NodeType*  returnType, NodeBlock* block);
NodeFunctionCall* newFunctionCallNode(NodeIdentifier* id, NodeExpression* args);
NodeExprStmt* newExprStmtNode(NodeExpression* node);
NodeReturnStmt* newReturnNode(NodeExpression* returnNode);
NodePlaceHolder* newPlaceHolderNode();
NodeUnaryOperator* newUnaryOperatorNode(NodeExpression* lhs, NODE_OP unaryOperatorType);
void replacePipeInput(NodeExpression** args, NodeExpression* newArg, int location);
NodeReturnEvaluated* newReturnEvaluated(NodeExpression* returnNode);
NodeArray* newArrayNode(NodeExpression* array);
void freeAllNodes();
int countAllocatedNodes();



extern void appendExprLinkedList(NodeExpression** head, NodeExpression* newMember);
extern void appendToStmtList(NodeStmtList* list, NodeStatement* newMember);
extern int getStmtListSize(NodeStmtList* list);
extern NodeStatement* indexStmtList(NodeStmtList* list, int index);


/*nodeImpl.cpp*/
extern int getExpressionLength(NodeExpression* expr);
extern void addDeclToList(NodeDeclList* list, NodeDecl* newDecl);
extern int getDeclListSize(NodeDeclList* list);
extern NodeDecl* indexDeclList(NodeDeclList* list, int index);
extern NodeExpression* indexExprList(NodeExpression* node, int index);
extern int checkAllExprTypes(NodeExpression* head, NODE_TYPE type);
extern ID_TYPE idTypeFromNodeType(NODE_TYPE nodeType);
extern int getStmtListSize(NodeStmtList* list);
extern NodeStatement* indexStmtList(NodeStmtList* list, int index);
extern int checkForPipeInput(NodeExpression* args);

//=============== FOR DEBUGGER ===============
extern void programToJson(std::vector<NodeStatement*>* head);


#endif

#ifndef NODE_H
#define NODE_H

typedef enum nodeOp {
    OP_PLUS, 
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD
} NODE_OP;


typedef enum idType {
    num,
    shape,
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
    TYPE,

    BLOCK,

    ASSIGN,

    BIN_OP,

    DOUBLE,
    SHAPE,

    STMT_LIST,
    DECL_LIST,
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
};

class NodeStatement: public Node {
    public:
        NodeStatement* nextStmt; 
};
class NodeExpression: public Node {};


class NodeType: public Node {
    public:
        ID_TYPE idType;
        NodeType(ID_TYPE idType): idType(idType) {
            this->nodeType = TYPE;
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
            char* idName
        ): idName(idName) {
            this->nodeType = ID;
        }
};


class NodeNumber: public NodeExpression {
    public:
        double value; 
        NodeNumber(double value): value(value) {
            this->nodeType = DOUBLE;
        }; 
};


class NodeShape: public NodeExpression {
    public:
        //TODO WHEN WE ADD OPEN CASCADE 
        NodeShape(){
            this->nodeType = SHAPE;
        }
};


class NodeBinaryOperator: public NodeExpression {
    public:
        NodeExpression* lhs;
        NodeExpression* rhs;
        NODE_OP binaryOperatorType;
        NodeBinaryOperator(NodeExpression* lhs,
               NodeExpression* rhs,
               NODE_OP binaryOperatorType
        ): lhs(lhs), rhs(rhs), binaryOperatorType(binaryOperatorType) {
            this->nodeType =  BIN_OP;
        }
};


/* 
   =======================================================
                     NODE STATEMENTS  
   =======================================================
*/  
class NodeDecl: public NodeStatement {
    public:
        NodeIdentifier* id;
        NodeType* type;
        NodeDecl* nextDecl;
        NodeExpression* value;
        NodeDecl(
            NodeIdentifier* id,
            NodeType* type,
            NodeExpression* value
        ): id(id), type(type), value(value) {
            this->nextStmt = NULL;
            this->nextDecl = NULL;
            this->nodeType = DECL;
        };
};

class NodeStmtList: public Node {
    public:
        NodeStatement* nextStmt; 
        NodeStmtList(NodeStatement* nextStmt)
            : nextStmt(nextStmt) {
            this->nodeType = STMT_LIST;
        }
};

class NodeDeclList: public Node {
    public:
        NodeDecl* nextDecl;
        NodeDeclList(NodeDecl* nextDecl)
            : nextDecl(nextDecl) {
            this->nodeType = DECL_LIST;
        }
};

class NodeBlock: public NodeStatement {
    public:
        NodeStmtList* stms;
        NodeBlock(
            NodeStmtList* stms
        ): stms(stms) {
            this->nextStmt = NULL;
            this->nodeType = BLOCK;
        }
};


class NodeFunction: public NodeStatement {
    public: 

        NodeIdentifier* id;
        NodeDeclList* arguments;
        NodeType*  returnType;
        NodeBlock* block;
        NodeFunction(
            NodeIdentifier* id,
            NodeDeclList* arguments,
            NodeType*  returnType,
            NodeBlock* block
        ): id(id), arguments(arguments), returnType(returnType), block(block) {
            this->nextStmt = NULL;
            this->nodeType = FUNCTION;
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


extern void appendToStmtList(NodeStmtList* list, NodeStatement* newMember);
extern int getStmtListSize(NodeStmtList* list);
extern NodeStatement* indexStmtList(NodeStmtList* list, int index);

extern void addDeclToList(NodeDeclList* list, NodeDecl* newDecl);
extern int getDeclListSize(NodeDeclList* list);
extern NodeDecl* indexDeclList(NodeDeclList* list, int index);

//=============== FOR DEBUGGER ===============
extern void programToJson(std::vector<NodeStatement*>* head);


#endif

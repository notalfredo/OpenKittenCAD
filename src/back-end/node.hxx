#ifndef NODE_H
#define NODE_H


typedef enum idType {
    num,
    shape
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

    BLOCK,

    ASSIGN,

    BIN_OP,

    DOUBLE,

    STMT_LIST,
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

class NodeIdentifier: public Node {
    public:
        char* idName;
        NodeIdentifier(
            char* idName
        ): idName(idName) {
            this->nodeType = ID;
        }
};


class NodeType: public Node {
    public:
        ID_TYPE idType;
        NodeType(ID_TYPE idType): idType(idType) {
            this->nodeType = ID;
        }
};


/* 
   =======================================================
                     NODE EXPRESSIONS
   =======================================================
*/  
class NodeDouble: public NodeExpression {
    public:
        double value; 
        NodeDouble(double value): value(value) {
            this->nodeType = DOUBLE;
        }; 
};


class NodeBinaryOperator: public NodeExpression {
    public:
        NodeExpression* lhs;
        NodeExpression* rhs;
        char binaryOperatorType;
        NodeBinaryOperator(NodeExpression* lhs,
               NodeExpression* rhs,
               char binaryOperatorType
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
        NodeDecl(
            NodeIdentifier* id,
            NodeType* type
        ): id(id), type(type) {
            this->nodeType = DECL;
            this->nextStmt = NULL;
            this->nextDecl = NULL;
        };
};

class NodeStmtList: public Node {
    public:
        NodeStatement* nextStmt; 
        NodeStmtList(NodeStatement* nextStmt)
            : nextStmt(nextStmt) {}
};

class NodeDeclList: public Node {
    public:
        NodeDecl* nextDecl;
        NodeDeclList(NodeDecl* nextDecl)
            : nextDecl(nextDecl) {}
};

class NodeBlock: public NodeStatement {
    public:
        NodeStmtList* stms;
        NodeBlock(
            NodeStmtList* stms
        ): stms(stms) {
            this->nodeType = BLOCK;
            this->nextStmt = NULL;
        }
};


class NodeFunction: public NodeStatement {
    public: 
        NodeDeclList* arguments;
        NodeBlock* block;
        NodeFunction(
            NodeDeclList* arguments,
            NodeBlock* block
        ): arguments(arguments), block(block) {
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

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

class NodeStatement: public Node {};
class NodeExpression: public Node {};


class NodeBlock: public NodeStatement {
    public:
        std::vector<NodeStatement*>* stms;
        NodeBlock(
            std::vector<NodeStatement*>* stms
        ): stms(stms) {
            this->nodeType = BLOCK;
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


class NodeIdentifier: public NodeExpression {
    public:
        char* idName;
        ID_TYPE type;
        NodeIdentifier(
            char* idName,
            ID_TYPE type
        ): idName(idName), type(type) {
            this->nodeType = ID;
        }
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

class NodeFunction: public NodeStatement {
    public: 
        std::vector<NodeIdentifier*>* arguments;
        NodeBlock* block;
        NodeFunction(
            std::vector<NodeIdentifier*>* arguments,
            NodeBlock* block
        ): arguments(arguments), block(block) {
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
            this->nodeType = ASSIGN;
        }
};


class NodeDecl: public NodeStatement {
    public:
        NodeIdentifier* id;
        ID_TYPE type;
        NodeExpression* assignment;
        NodeDecl(
            NodeIdentifier* id,
            ID_TYPE type,
            NodeExpression* assignment
        ): id(id), type(type), assignment(assignment) {
            this->nodeType = DECL;
        };
};


class NodeRepeat: public NodeStatement {
    public:
        NodeBlock* block;
        NodeRepeat(NodeBlock* block): block(block) {
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
            this->nodeType = FOR;
        };

};
 

class NodeElse: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeElse(NodeExpression* condition): condition(condition) {
            this->nodeType = ELSE;
        };
};


class NodeElif: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeElif(NodeExpression* condition, NodeBlock* block): 
            condition(condition), block(block) {
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
            this->nodeType = IF;
        };
};


//=============== FOR DEBUGGER ===============
extern void programToJson(std::vector<NodeStatement*>* head);


#endif

#ifndef NODE_H
#define NODE_H

typedef enum idType {
} ID_TYPE;


#include <string>
#include <vector>

class Node {
    public:
        virtual ~Node() {}
};

class NodeStatement: public Node {};
class NodeExpression: public Node {};


class NodeBlock: public NodeStatement {
    std::vector<NodeStatement> stms;
    NodeBlock();
};


/* 
   =======================================================
                     NODE EXPRESSIONS
   =======================================================
*/  
class NodeDouble: public NodeExpression {
    public:
        double value; 
        NodeDouble(double value): value(value) {}; 
};


class NodeIdentifier: public NodeExpression {
    public:
        std::string idName;
        NodeIdentifier(std::string idName): idName(idName) {}
};


class NodeBinaryOperator: public NodeExpression {
    public:
        NodeExpression* lhs;
        NodeExpression* rhs;
        char binaryOperatorType;
        NodeBinaryOperator(NodeExpression* lhs,
               NodeExpression* rhs,
               char binaryOperatorType
        ): lhs(lhs), rhs(rhs), binaryOperatorType(binaryOperatorType) {}
};


/* 
   =======================================================
                     NODE STATEMENTS  
   =======================================================
*/  


class NodeBinaryAssign: public NodeExpression {
    public:
        NodeIdentifier id;
        NodeExpression* assignment;
        NodeBinaryAssign(NodeIdentifier id,
                         NodeExpression* assignment
        ): id(id), assignment(assignment){}
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
        ): id(id), type(type), assignment(assignment) {};
};


class NodeRepeat: public NodeStatement {
    public:
        NodeBlock* block;
        NodeRepeat(NodeBlock* block): block(block) {};
};


class NodeUntil: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeUntil(
            NodeExpression* condition,
            NodeBlock* block
        ): condition(condition), block(block)  {};
};


class NodeWhile: public NodeStatement {
    public:
        NodeExpression* condition; 
        NodeBlock* block;
        NodeWhile(
            NodeExpression* condition,
            NodeBlock* block
        ): condition(condition), block(block)  {};

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
        ): localVal(localVal), condition(condition), update(update)  {};

};
 

class NodeElse: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeElse(NodeExpression* condition): condition(condition) {};
};


class NodeElif: public NodeStatement {
    public:
        NodeExpression* condition;
        NodeBlock* block;
        NodeElif(NodeExpression* condition, NodeBlock* block): 
            condition(condition), block(block) {};
};


class NodeIf: public NodeStatement {
    public:
        NodeExpression* condition;
        std::vector<NodeElif*> elif;
        NodeBlock* block;
        NodeIf(NodeExpression* condition, NodeBlock* block):
            condition(condition), block(block) {};
};
#endif

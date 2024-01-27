#include <string>

class Node {
    public:
        virtual ~Node() {}
};

class NodeStatement: public Node {};
class NodeExpression: public Node {};


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

class NodeBinaryAssign: public NodeExpression {
    public:
        NodeIdentifier id;
        NodeExpression* assignment;
        NodeBinaryAssign(NodeIdentifier id,
                         NodeExpression* assignment
        ): id(id), assignment(assignment){}
};

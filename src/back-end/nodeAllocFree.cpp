#include "node.hxx"

/* 
 * This linked list node is a "trail" of all the nodes types from
 * this file that have ever been allocated. The first node 
 * allocated will point to NULL. The second node will point to
 * the first node allocated and so on. 
 *
 * TLDR: _prevAlloc is pointer to last allocated node
*/
static Node* _prevAlloc = NULL;


NodeType* newNodeType(ID_TYPE idType)
{
    NodeType* me = new NodeType(idType, _prevAlloc);  
    _prevAlloc = me;
    return me; 
}


NodeIdentifier* newIdentifierNode(char* idName)
{
    NodeIdentifier* me = new NodeIdentifier(idName, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeNumber* newNumberNode(double value)
{
    NodeNumber* me = new NodeNumber(value, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeShape* newNodeShape()
{
    NodeShape* me = new NodeShape(_prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeBinaryOperator* newBinaryOperatorNode(NodeExpression* lhs, NodeExpression* rhs, NODE_OP binaryOperatorType)
{
    NodeBinaryOperator* me = new NodeBinaryOperator(
                                lhs,
                                rhs,
                                binaryOperatorType,
                                _prevAlloc
                                ); 
    _prevAlloc = me; 
    return me;
}


NodeDecl* newDeclNode(NodeIdentifier* id, NodeType* type, NodeExpression* value)
{
    NodeDecl* me = new NodeDecl(id, type, value, _prevAlloc);
    _prevAlloc = me;

    return me;
}


NodeStmtList* newStmtList(NodeStatement* nextStmt)
{
    NodeStmtList* me = new NodeStmtList(nextStmt, _prevAlloc);
    _prevAlloc = me;
    return me;
}

NodeDeclList* newDeclList(NodeDecl* nextDecl)
{
    NodeDeclList* me = new NodeDeclList(nextDecl, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeBlock* newBlock(NodeStmtList* stmts)
{
    NodeBlock* me = new NodeBlock(stmts, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeFunction* newFunctionNode(NodeIdentifier* id, NodeDeclList* arguments, NodeType*  returnType, NodeBlock* block)
{
    NodeFunction* me = new NodeFunction(id, arguments, returnType, block);
    _prevAlloc = me;
    return me;
}



void _freeNode(Node* node) {
    switch (node->nodeType) {
        case ID: {
            NodeIdentifier* cast = static_cast<NodeIdentifier*>(node);
            free(cast->idName);
            return;
        }
        default: {
            return;
        }
    }
    free(node);
}

void freeAllNodes()
{
    int freedNodeCount = 0; 

    Node* curr = _prevAlloc;      
    while (curr){
        Node* temp = curr->_allocatedLinkedList;
        free(curr);
        curr = temp;
    }
}
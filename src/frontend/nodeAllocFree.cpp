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


NodeExprStmt* newExprStmtNode(NodeExpression* node)
{
    NodeExprStmt* me = new NodeExprStmt(node, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodePlaceHolder* newPlaceHolderNode()
{
    NodePlaceHolder* me = new NodePlaceHolder(_prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeFunctionCall* newFunctionCallNode(NodeIdentifier* id, NodeExpression* args)
{
    NodeFunctionCall* me = new NodeFunctionCall(id, args, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeType* newNodeType(ID_TYPE idType)
{
    NodeType* me = new NodeType(idType, _prevAlloc);  
    _prevAlloc = me;
    return me; 
}


NodeBlock* newNodeBlock(NodeStmtList* stmts)
{
    NodeBlock* me = new NodeBlock(stmts, _prevAlloc);
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


NodePoint* newNodePoint()
{
    NodePoint* me = new NodePoint(_prevAlloc);
    _prevAlloc = me;
    return me;
}

NodeEdge* newNodeEdge()
{
    NodeEdge* me = new NodeEdge(_prevAlloc);
    _prevAlloc = me;
    return me;
}

NodeShape* newNodeShape(OCCT_SHAPE shape)
{
    NodeShape* me = new NodeShape(shape, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeArray* newArrayNode(NodeExpression* array)
{
    NodeArray* me = new NodeArray(array, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeUnaryOperator* newUnaryOperatorNode(NodeExpression* lhs, NODE_OP unaryOperatorType)
{
    NodeUnaryOperator* me = new NodeUnaryOperator(lhs, unaryOperatorType, _prevAlloc); 
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


NodeDecl* newDeclNode(NodeIdentifier* id, NodeType* type, NodeExpression* value, DECL_MUT_STATE mutState)
{
    NodeExpression* temp = value;
    if(!temp){
        switch(type->idType){
            case num: {
                //TODO
            }
            case shape: {
                //TODO 
            }
            case _void: {
                //TODO 
            }
            case point: {
                //TODO
            }
            case edge: {
                //TODO
            }
        }
    }


    NodeDecl* me = new NodeDecl(id, type, temp,  mutState, _prevAlloc);
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


NodeFunction* newFunctionNode(NodeIdentifier* id, NodeDeclList* arguments, NodeType*  returnType, NodeBlock* block)
{
    NodeFunction* me = new NodeFunction(id, arguments, returnType, block, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeReturnStmt* newReturnNode(NodeExpression* returnNode)
{
    NodeReturnStmt* me = new NodeReturnStmt(returnNode, _prevAlloc);
    _prevAlloc = me;
    return me;
}


NodeReturnEvaluated* newReturnEvaluated(NodeExpression* returnNode)
{
    NodeReturnEvaluated* me = new NodeReturnEvaluated(returnNode, _prevAlloc);
    _prevAlloc = me;
    return me;
}


void _freeNode(Node* node) {
    switch (node->nodeType) {
        case ID: {
            NodeIdentifier* cast = static_cast<NodeIdentifier*>(node);
            free(cast->idName);
            break;
        }
        case SHAPE: {
            NodeShape* cast = static_cast<NodeShape*>(node);

            if(cast->brepShape){
                delete cast->brepShape;
            }

            cast->brepShape = NULL;
            cast->shape = NULL;
            break;
        }
        case EDGE: {
            NodeEdge* cast = static_cast<NodeEdge*>(node);

            if (cast->edgeType == type_edge) {
                delete cast->brepEdge;
                cast->edge = NULL;
            }
            else if (cast->edgeType == type_wire) {
                delete cast->brepWire;
                cast->wireShape = NULL;
            }
            break;
        }
        case POINT: {
            NodePoint* pnt = static_cast<NodePoint*>(node);

            if(pnt->point){
                free(pnt->point);
            }
        }
        default: {
            break;
        }
    }
    delete node;
}

void freeAllNodes()
{
    int freedNodeCount = 0; 

    Node* curr = _prevAlloc;      
    while (curr){
        Node* temp = curr->_allocatedLinkedList;
        _freeNode(curr);
        curr = temp;
        _prevAlloc = curr;
        freedNodeCount += 1;
    }
    fprintf(stderr, "\n--> Freed %d nodes\n", freedNodeCount);
}

/* For testing purposes */
int countAllocatedNodes()
{
    Node* curr = _prevAlloc;
    int count = 0;

    while (curr){
        count += 1; 
        curr = curr->_allocatedLinkedList;
    }
    return count;
}

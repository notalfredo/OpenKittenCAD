#include "enumToString.hxx"
#include "node.hxx"
#include <iostream>

void appendExprLinkedList(NodeExpression** head, NodeExpression* newMember)
{
    if(!newMember){
        fprintf(stderr, "Trying to append NULL to expr linked list exiting ... \n");
        exit(1);
    }

    NodeExpression* temp = *head;
    
    if(!temp){
        *head = newMember;
        return;
    }

    while(temp->nextExpr){
        temp = temp->nextExpr;
    }
    temp->nextExpr = newMember;
}

int getExpressionLength(NodeExpression* expr)
{
    NodeExpression* tempHead = expr;
    int count = 0;

    while(tempHead) {
        count++;
        tempHead = tempHead->nextExpr;
    }

    return count;
}


NodeExpression* indexExprList(NodeExpression* node, int index)
{
    NodeExpression* temp = node;
    int count = 0;

    while(temp){
        if(count == index){
            return temp;
        }
        count++;
        temp = temp->nextExpr;
    }
    
    return temp;
}


void appendToStmtList(NodeStmtList* list, NodeStatement* newMember)
{
    NodeStatement* curr = list->nextStmt;
    while (curr->nextStmt) {
        curr = curr->nextStmt;
    }
    curr->nextStmt = newMember;
}


int getStmtListSize(NodeStmtList* list)
{
    int count = 0;
    NodeStatement* curr = list->nextStmt;
    while (curr) {
        count += 1;
        curr = curr->nextStmt;
    }
    return count;
}


NodeStatement* indexStmtList(NodeStmtList* list, int index)
{
    int count = 0;
    NodeStatement* curr = list->nextStmt;

    if(!curr){
        return NULL;
    }

    while (curr) {
        if(index == count){
            return curr;
        }
        count += 1;
        curr = curr->nextStmt;
    }

    return curr;
}


void addDeclToList(NodeDeclList* list, NodeDecl* newDecl)
{
    NodeDecl* curr = list->nextDecl;

    while(curr->nextDecl){
        curr = curr->nextDecl;
    }

    curr->nextDecl = newDecl;
}


int getDeclListSize(NodeDeclList* list)
{
    int size = 0;
    NodeDecl* curr = list->nextDecl; 

    while(curr){
        size += 1;
        curr = curr->nextDecl;
    }

    return size; 
}


NodeDecl* indexDeclList(NodeDeclList* list, int index)
{
    int count = 0;
    NodeDecl* curr = list->nextDecl; 

    while(curr){
        if(count == index){
            return curr;
        }

        count += 1;
        curr = curr->nextDecl;
    }

    return NULL;
}


ID_TYPE idTypeFromNodeType(NODE_TYPE nodeType)
{
    switch(nodeType){
        case DOUBLE:{
            return num;
        }
        case SHAPE: {
            return shape;
        }
        default: {
            fprintf(stderr, "Can only pass numbers and shapes to functions you tried to pass %s, exiting...\n", nodeTypeToString(nodeType));
            exit(1);
        }
    }
    return num; //Make compiler happy
}



/*
 * When we are pipeing check if func to be piped
 * can take pipe input. 
 *
 * Returns the index if and only if there exist 
 * one pipe input. 
 *
 * Return -1 if no pipe input can be taken 
 * return -2 if more than one pipe input can be taken
*/
int checkForPipeInput(NodeExpression* args)
{
    NodeExpression* list = args; 
    int count = 0;
    int location = 0;
    int numberOfPipesFound = 0;

    while(list){
        if(list->nodeType == PLACEHOLDER){
            if(numberOfPipesFound >= 1){
                return -2;
            }
            numberOfPipesFound++;
            count++; 
            location = count;
        }
        
        list = list->nextExpr;
    }

    if(numberOfPipesFound == 1){
        return location;
    }

    return -1;
}

void replacePipeInput(NodeExpression** args, NodeExpression* newArg, int location)
{
    int count = 0; 
    NodeExpression* curr = *args;

    if(location == 0){
        newArg->nextExpr = (*args)->nextExpr;
        (*args)->nextExpr = NULL;
        *args = newArg;
    }
    count++;
    
    while(curr && ((count + 1) != location)){
        curr = curr->nextExpr;
        count++;
    }

    if(curr->nextExpr->nodeType != PLACEHOLDER){
        fprintf(stderr, "Expected next node to be a placehodler\n"); 
        exit(1);
    }

    NodeExpression* pipeNode = curr->nextExpr;

    curr->nextExpr = newArg;
    newArg->nextExpr = pipeNode->nextExpr;
    pipeNode->nextExpr = NULL;

}






#include "node.hxx"
#include <iostream>


void appendToStmtList(NodeStmtList* list, NodeStatement* newMember)
{
    NodeStatement* temp = list->nextStmt;
    while (temp) {
        temp = temp->nextStmt;
    }
    temp->nextStmt = newMember;
}


int getStmtListSize(NodeStmtList* list)
{
    int count = 0;

    NodeStatement* temp = list->nextStmt;
    while (temp) {
        count += 1;
        temp = temp->nextStmt;
    }
    return count;
}

NodeStatement* indexStmtList(NodeStmtList* list, int index)
{
    int count = 0;
    NodeStatement* temp = list->nextStmt;

    while (temp) {
        if(index == count){
            return temp;
        }
        count += 1;
        temp = temp->nextStmt;
    }
    return NULL;
}


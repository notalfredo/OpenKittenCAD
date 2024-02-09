#include "node.hxx"
#include <iostream>


void appendToStmtList(NodeStmtList* list, NodeStatement* newMember)
{
    NodeStatement* curr = list->nextStmt;
    while (curr) {
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

    while (curr) {
        if(index == count){
            return curr;
        }
        count += 1;
        curr = curr->nextStmt;
    }

    return NULL;
}


void addDeclToList(NodeDeclList* list, NodeDecl* newDecl)
{
    NodeDecl* curr = list->nextDecl;

    while(curr){
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

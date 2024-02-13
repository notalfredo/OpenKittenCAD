#include "semantic.hxx"
#include "node.hxx"
#include <iostream>
#include "tau.h"

typedef struct C_String {
    const char* name;
} C_string;

const C_String names[] = {
    {"name_1"},
    {"name_2"},
    {"name_3"},
    {"name_4"},
    {"name_5"},
    {"name_6"},
    {"name_7"},
    {"name_8"},
    {"name_9"},
};

const int NAMES_SIZE = sizeof(names) / sizeof(C_string);


void insertIntoTopBlock(SymbolTableHead* symTable)
{
    for(int i = 0; i < NAMES_SIZE; i++){
        NodeDecl* declNode = new NodeDecl(
            new NodeIdentifier(strdup(names[i].name)),   
            new NodeType(num),   
            new NodeNumber(i)
        );
        insertSymbolFromNode(symTable, declNode);

        free(declNode->id->idName);
        delete declNode->type;
        delete declNode->id;
        delete declNode->value;
        delete declNode;
    }
}



TAU_MAIN()



TEST(symTable, valgrindMemmoryLeakDetector) { 
    SymbolTableHead* symbolTable = newSymbolTable();

    insertIntoTopBlock(symbolTable);
    insertIntoTopBlock(symbolTable);
    insertIntoTopBlock(symbolTable);
    insertIntoTopBlock(symbolTable);

    dumpSymbolTable(symbolTable);

    freeSymbolTable(&symbolTable);
}




    //CHECK_EQ(1, getCurrentSize(symbolTable));


    //appendNewBasicBlock(&symbolTable);

    //fprintf(stderr, "-----------------\n");
    //insertIntoTopBlock(symbolTable);
    //dumpSymbolTable(symbolTable);
    //fprintf(stderr, "-----------------\n");

    //freeTopBlock(&symbolTable, 0);

    //fprintf(stderr, "-----------------\n");
    //dumpSymbolTable(symbolTable);
    //fprintf(stderr, "-----------------\n");


    //appendNewBasicBlock(&symbolTable);
    //insertIntoTopBlock(symbolTable);
    //dumpSymbolTable(symbolTable);

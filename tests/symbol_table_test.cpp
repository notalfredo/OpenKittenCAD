#include "symbolTable.hxx"
#include "node.hxx"
#include <iostream>
#include "tau.h"

typedef struct C_String {
    const char* name;
} C_string;

const C_String firstNameSuite[] = {
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

const int FIRST_SUITE_SIZE = sizeof(firstNameSuite) / sizeof(C_string);

const C_String secondNameSuite[] = {
    {"name_10"},
    {"name_11"},
    {"name_12"},
    {"name_13"},
    {"name_14"},
    {"name_15"},
    {"name_16"},
    {"name_17"},
    {"name_18"},
    {"name_19"},
};

const int SECOND_SUITE_SIZE = sizeof(secondNameSuite) / sizeof(C_string);


void insertIntoTopBlock(SymbolTableHead* symTable, int suiteNum)
{
    
    int size = suiteNum == 1 ? FIRST_SUITE_SIZE : SECOND_SUITE_SIZE;

    for(int i = 0; i < size; i++){
        NodeIdentifier* idNode = suiteNum == 1 ? newIdentifierNode(strdup(firstNameSuite[i].name)) : newIdentifierNode(strdup(secondNameSuite[i].name));

        NodeDecl* declNode = newDeclNode(
            idNode,
            newNodeType(num),   
            newExprStmtNode(newNumberNode(i))
        );
        insertSymbolFromNode(symTable, declNode);

        free(declNode->id->idName);
        delete declNode->type;
        delete declNode->id;
        delete declNode->value->expr;
        delete declNode->value;
        delete declNode;
    }
}



TAU_MAIN()



TEST(symTable, valgrindMemmoryLeakDetectorOne) { 
    SymbolTableHead* symbolTable = newSymbolTable();

    insertIntoTopBlock(symbolTable, 1);
    insertIntoTopBlock(symbolTable, 1);
    insertIntoTopBlock(symbolTable, 1);
    insertIntoTopBlock(symbolTable, 1);

    dumpSymbolTable(symbolTable);

    freeSymbolTable(&symbolTable);
    resetBlockCounter();
}


TEST(symTable, valgrindMemmoryLeakDetectorTwo) {
    SymbolTableHead* symbolTable = newSymbolTable();

    insertIntoTopBlock(symbolTable, 0);
    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 1);
    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 1);
    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 0);
    appendNewBasicBlock(&symbolTable);

    dumpSymbolTable(symbolTable);
    freeSymbolTable(&symbolTable);
    resetBlockCounter();
}


TEST(symTable, testSymbolTableSize) { 
    SymbolTableHead* symbolTable = newSymbolTable();

    insertIntoTopBlock(symbolTable, 0);
    CHECK_EQ(1, getCurrentSize(symbolTable));

    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 1);
    CHECK_EQ(2, getCurrentSize(symbolTable));

    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 0);
    CHECK_EQ(3, getCurrentSize(symbolTable));

    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 1);
    CHECK_EQ(4, getCurrentSize(symbolTable));

    freeTopBlock(&symbolTable);
    CHECK_EQ(3, getCurrentSize(symbolTable));

    freeTopBlock(&symbolTable);
    CHECK_EQ(2, getCurrentSize(symbolTable));

    freeTopBlock(&symbolTable);
    CHECK_EQ(1, getCurrentSize(symbolTable));

    freeTopBlock(&symbolTable);
    CHECK_EQ(0, getCurrentSize(symbolTable));

    dumpSymbolTable(symbolTable);
    freeSymbolTable(&symbolTable);
    resetBlockCounter();
}

TEST(symTable, testFindSymbol) { 
    SymbolTableHead* symbolTable = newSymbolTable();

    insertIntoTopBlock(symbolTable, 2);
    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 1);

    CHECK_EQ(2, containsSymbolName(symbolTable, "name_1"));
    CHECK_EQ(1, containsSymbolName(symbolTable, "name_10"));
    CHECK_EQ(0, containsSymbolName(symbolTable, "invalid_name"));

    freeTopBlock(&symbolTable);
    appendNewBasicBlock(&symbolTable);
    insertIntoTopBlock(symbolTable, 2);
    CHECK_EQ(3, containsSymbolName(symbolTable, "name_18"));
    CHECK_EQ(0, containsSymbolName(symbolTable, "wat"));

    dumpSymbolTable(symbolTable);
    freeSymbolTable(&symbolTable);
    resetBlockCounter();
}



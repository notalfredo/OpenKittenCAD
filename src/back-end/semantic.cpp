#include "semantic.hxx"

SymbolTableHead* newSymbolTable()
{
    SymbolTableHead* symTable = (SymbolTableHead*)calloc(1, sizeof(SymbolTableHead));
    symTable->next = NULL;
    return symTable;
}


void appendBasicBlock(SymbolTableHead* symTable)
{
    struct BasicBlock* newBlock = (BasicBlock*)calloc(1, sizeof(BasicBlock));
    struct BasicBlock* currHighestBlockHead = symTable->next;

    newBlock->next = currHighestBlockHead;     
    symTable->next = newBlock;
}


void addSymbol( )
{


}

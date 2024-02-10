#ifndef DEBUGGER_HELPER_FUNC
#define DEBUGGER_HELPER_FUNC
#include <stdio.h>
#include <vector>
#include "node.hxx"

std::string nodeTypeToString(NODE_TYPE type);
std::string idTypeToString(ID_TYPE type);
std::string stringFromNodeOp(NODE_OP type);

#endif

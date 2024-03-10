#ifndef ENUM_TO_STRING_H
#define ENUM_TO_STRING_H

#include "node.hxx"

extern const char* nodeTypeToString(NODE_TYPE type);
extern const char* nodeOpToString(NODE_OP nodeOP);
extern const char* idTypeTostring(ID_TYPE type);
extern const char* numToStrPlace(int num);
extern ID_TYPE nodeTypeFromIdType(nodeType rhs);

#endif

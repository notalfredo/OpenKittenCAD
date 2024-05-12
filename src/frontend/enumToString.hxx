#ifndef ENUM_TO_STRING_H
#define ENUM_TO_STRING_H

#include "BRepBuilderAPI_WireError.hxx"
#include "node.hxx"

extern const char* nodeTypeToString(NODE_TYPE type);
extern const char* nodeOpToString(NODE_OP nodeOP);
extern const char* idTypeTostring(ID_TYPE type);
extern const char* numToStrPlace(int num);
extern ID_TYPE nodeTypeFromIdType(nodeType rhs);
extern const char* wireContructionError(BRepBuilderAPI_WireError errorMsg);

#endif

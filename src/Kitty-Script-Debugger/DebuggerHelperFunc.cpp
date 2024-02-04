#include "DebuggerHelperFunc.hxx"



std::string nodeTypeToString(NODE_TYPE type){
    switch (type) {
        case IF: {
            return "IF";
        }
        case ELIF: {
            return "ELIF";
        }
        case ELSE: {
            return "ELSE";
        }
        case FOR: {
            return "FOR";
        }
        case WHILE: {
            return "WHILE";
        }
        case REPEAT: {
            return "REPEAT";
        }
        case UNTIL: {
            return "UNTIL";
        }
        case DECL: {
            return "DECL";
        }
        case STMT: {
            return "STMT";
        }
        case FUNCTION: {
            return "FUNCTION";
        }
        case ID: {
            return "ID";
        }
        case BLOCK: {
            return "BLOCK";
        }
        case ASSIGN: {
            return "ASSIGN";
        }
        case BIN_OP: {
            return "BIN_OP";
        }
        case DOUBLE: {
            return "DOUBLE";
        }
        default: {
            return "UNKOWN NODE TYPE";
        }
    }
}

#include "TreeDumpHelper.hxx"

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
        case DECL_LIST: {
            return "DECL_LIST";
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


std::string idTypeToString(ID_TYPE type){
    switch (type) {
        case num: {
            return "num";
        }
        case shape: {
            return "shape";
        }
        case _void: {
            return "void";
        }
        default: {
            return "HIT DEFAULT IN idTypeToString(ID_TYPE type)";
        }
    }
}


std::string stringFromNodeOp(NODE_OP type){
    switch (type) {
        case OP_PLUS: {
            return "PLUS";
        }
        case OP_SUB: {
            return "SUB";
        }
        case OP_MUL: {
            return "MUL";
        }
        case OP_DIV: {
            return "DIV";
        }
        case OP_MOD: {
            return "MOD";
        }
        default: {
            return "IN stringFromNodeOp HIT DEFAULT CASE";
        }
    }
}

#include "node.hxx"


const char* nodeTypeToString(NODE_TYPE type)
{  
    switch(type) {
        case IF:             { return "IF";            }
        case ELIF:           { return "ELIF";          }
        case ELSE:           { return "ELSE";          }
        case FOR:            { return "FOR";           }
        case WHILE:          { return "WHILE";         }
        case REPEAT:         { return "REPEAT";        }
        case UNTIL:          { return "UNTIL";         }
        case DECL:           { return "DECl";          }
        case STMT:           { return "STMT";          }
        case FUNCTION:       { return "FUNCTION";      }
        case ID:             { return "ID";            }
        case FUNCTION_CALL:  { return "FUNCTION_CALL"; }
        case TYPE:           { return "TYPE";          }
        case BLOCK:          { return "BLOCK";         }
        case ASSIGN:         { return "ASSIGN";        }
        case BIN_OP:         { return "BIN_OP";        }
        case DOUBLE:         { return "DOUBLE";        }
        case SHAPE:          { return "SHAPE";         }
        case STMT_LIST:      { return "BIN_OP";        }
        case DECL_LIST:      { return "DOUBLE";        }
        case EXPR_STMT:      { return "EXPR_STMT";     }
        case POINT:          { return "POINT";         }
        default: {
            fprintf(stderr, "Hit default case in nodeTypeToString() exiting...\n");
            exit(0);
        }
    }
    //Make the compiler happy
    return NULL;
}

const char* nodeOpToString(NODE_OP nodeOp)
{
    switch (nodeOp) {
        case OP_PLUS:   { return "+";    } 
        case OP_SUB:    { return "-";    }
        case OP_MUL:    { return "*";    }
        case OP_DIV:    { return "/";    }
        case OP_MOD:    { return "mod";  }
        case OP_ASSIGN: { return ":=";   }
        default: {
            fprintf(stderr, "Hit default case in nodeTypeToString() exiting...\n");
            exit(0);
        }
    }
    //Make the compiler happy
    return NULL;
}

const char* idTypeTostring(ID_TYPE type){
    switch(type){
        case num: {
            return "number";
        }
        case shape: {
            return "shape";
        }
        case _void: {
            return "void";
        }
        case point: {
            return "point";
        }
        case edge: {
            return "edge";
        }
    }
    //Make the compiler happy
    return NULL;
}


const char* numToStrPlace(int num){
    if(num == 1){
        return "st";
    }
    else if(num == 2){
        return "nd";
    }
    else if(num == 3){
        return "rd";
    }
    return "th";
}

ID_TYPE nodeTypeFromIdType(nodeType rhs)
{
    switch(rhs){
        case DOUBLE: {
            return num;
        }
        case SHAPE: {
            return shape;
        }
        case POINT: {
            return point;
        }
        default: {
            fprintf(stderr, "No mapping from %s, to ID_TYPE\n", nodeTypeToString(rhs));
            exit(1);
        }
    }

}




#include "node.hxx"


const char* nodeTypeToString(NODE_TYPE type)
{  
    switch(type) {
        case IF:             { return "IF\n";       }
        case ELIF:           { return "ELIF\n";     }
        case ELSE:           { return "ELSE\n";     }
        case FOR:            { return "FOR\n";      }
        case WHILE:          { return "WHILE\n";    }
        case REPEAT:         { return "REPEAT\n";   }
        case UNTIL:          { return "UNTIL\n";    }
        case DECL:           { return "DECl\n";     }
        case STMT:           { return "STMT\n";     }
        case FUNCTION:       { return "FUNCTION\n"; }
        case ID:             { return "ID\n";       }
        case FUNCTION_CALL:  { return "ID\n";       }
        case TYPE:           { return "TYPE\n";     }
        case BLOCK:          { return "BLOCK\n";    }
        case ASSIGN:         { return "ASSIGN\n";   }
        case BIN_OP:         { return "BIN_OP\n";   }
        case DOUBLE:         { return "DOUBLE\n";   }
        case SHAPE:          { return "SHAPE\n";    }
        case STMT_LIST:      { return "BIN_OP\n";   }
        case DECL_LIST:      { return "DOUBLE\n";   }
        case EXPR_STMT:      { return "EXPR_STMT\n";    }
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




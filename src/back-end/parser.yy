    /* Prologue */
%require "3.2"
%langauge "c++"
%defines


%define api.token.constructor
%define api.value.type variant
%define parse.assert


%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
    #include "node.hxx"
}


%define api.token.prefix {TOK_}
%token
    ASSIGN ":="
    PIPE   "|>"
    EQ     "=="
    NE     "!="
    GE     ">="
    LE     "<="
;



%nterm <NodeDouble*> NUM
    
%% 
    /* Grammer rules */



%%

    /* Epilogue */

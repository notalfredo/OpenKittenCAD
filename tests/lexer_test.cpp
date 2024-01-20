#include <cstdlib>
#include <iostream>
#include <keywords.hxx>
#include "tau.h"
#include <unistd.h>
#include "lexer.yy.h"



TAU_MAIN()
    
TEST(lexer, input) { 

    std::string str((const char*)"/tests/input_tests/input.txt");
    std::string myPathString = MY_PATH +  str;
    char* pathCharPnt = (char*)calloc(myPathString.length(), sizeof(char));
    strcpy(pathCharPnt, myPathString.c_str());

    FILE* file = fopen(pathCharPnt, "r");

    if(file == NULL){
        std::cout << "File was unable to open file: " << " " << pathCharPnt << " " << std::endl;
        CHECK(0);
        exit(0);
    }


    //yyscan_t scanner;
    //int tok;

    //yylex_init(&scanner);
    //yyrestart( file, scanner );
    //yyset_lineno( 1, scanner );


    //while ((tok=yylex(scanner)) > 0){
    //    printf("tok=%d yytext=%s\n", tok, yyget_text(scanner));
    //}
    //yylex_destroy(scanner);
}

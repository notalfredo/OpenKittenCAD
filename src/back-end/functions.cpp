#include "functions.hxx"
#include <cstdio>
#include <cstring>
#include <system_error>

KnownFunctions funs[] {
    {"print", _print}
};

const int SIZE_OF_FUNS = sizeof(funs) / sizeof (KnownFunctions);

void _print(double num)
{
    fprintf(stdout, "%f\n", num);
}

void lookUpFunc(const char * funName)
{
    for(int index = 0; index < SIZE_OF_FUNS; index++){
        if(!strcmp(funs[index].name, funName)){

        }
    }
}

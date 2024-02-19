#ifndef FUNCTIONS_H
#define FUNCTIONS_H


union functionPointers {
    void (*println)(double); 
};


typedef struct KnownFunctions{
    const char* name;
    functionPointers funPtr;
}KnownFunctions;


void _print(double num);
void lookUpFunc(const char*);
void execFunc(const char*, void* paramInfo);

#endif

#include "node.hxx"
#include "functions.hxx"
#include "tau.h"



TAU_MAIN()


TEST(functions, testOne){
    function* func = lookUpFunc("print");
    if(!func){
        CHECK(0);
    }
    execFunc(func, newNumberNode(10));
}

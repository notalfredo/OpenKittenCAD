#include "node.hxx"
#include "functions.hxx"
#include "tau.h"



TAU_MAIN()


TEST(functions, testOne){
    functionPtr* func = lookUpFunc("print");
    if(!func){
        CHECK(0);
    }
    std::vector<NodeExpression*> vec = {newNumberNode(10)};

    execFunc(func, vec);



    freeAllNodes();
}

TEST(functions, testTwo){
    functionPtr* func = lookUpFunc("sphere");
    if(!func){
        CHECK(0);
    }

    std::vector<NodeExpression*> vec = {newNumberNode(10)};
    execFunc(func, vec);



    freeAllNodes();
}

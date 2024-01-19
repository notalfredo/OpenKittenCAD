#include "tau/tau.h"
#include "lexer.yy.h"
//#include <keywords.hxx>

TAU_MAIN()

TEST(a, count) {
    REQUIRE_EQ((int)tok_IF, (int)tok_IF);
}

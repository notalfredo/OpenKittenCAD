#include <string.h>

#include <FlexLexer.h>

#include <iostream>  
#include <sstream>  
#include <string>    
#include <streambuf>
#include <fstream> 

int main() {
    std::string stringvalues = "TOKEN TEST 123";
    std::istringstream inputStringStream(stringvalues);

    std::istream& inputStream = inputStringStream;
    std::ofstream outputStream("test.txt");

    yyFlexLexer myTest(inputStream, outputStream);

    int tok;
    while ( ( tok = myTest.yylex() ) ) {
        std::cout << tok << std::endl; 
    }
}

#include <string.h>


//#include <FlexLexer.h>



#include <iostream>     // std::cout
#include <sstream>      // std::istringstream
#include <string>       // std::string
#include <streambuf>
#include <fstream> 
                        
#include "keywords.hxx"

//extern int  yycolno;
//extern int  numIllegalChars;

int main() {


    std::string stringvalues = "tokenize this";
    std::istringstream inputStringStream(stringvalues);

    std::string stringvalues2 = "             ";
    std::istringstream outputStringStream(stringvalues2);


    std::istream& inputStream = inputStringStream;
    std::ofstream outputStream("test.txt");


    //yyFlexLexer myTest(inputStream, outputStream);


}

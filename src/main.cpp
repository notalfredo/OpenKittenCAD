#include "GlfwOcctView.h"
#include "./frontend/frontEndEntryPoint.hxx"

int main ()
{
  //GlfwOcctView anApp;
  //try
  //{
  //  anApp.run();
  //}
  //catch (const std::runtime_error& theError)
  //{
  //  std::cerr << theError.what() << std::endl;
  //  return EXIT_FAILURE;
  //}
  //return 0;
  
    char message[16] = "test message";

    programResponse* resp = frontEndEntryPoint(message, string);

    std::cout << resp->message << std::endl;
}

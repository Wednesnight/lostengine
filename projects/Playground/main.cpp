#include <iostream>
#include <stdexcept>
#include "lost/application/Application.h"
#include "Controller.h"

using namespace lost::application;

int main(int argn, char** args)
{
  int result = EXIT_SUCCESS;
  try
  {
    static ApplicationPtr app = Application::create();
    app->addTasklet(TaskletPtr(new Controller(app)));
    app->run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
    result = EXIT_FAILURE;
  }        
  return result;
}

#include <iostream>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "MeshTest.h"

using namespace std;
using namespace lost;
using namespace lost::application;

int main(int argn, char** args)
{
  try
  {
    ApplicationPtr app = Application::create(TaskletPtr(new MeshTest));
    app->run();      
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}


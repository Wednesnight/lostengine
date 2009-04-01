#include "lost/application/Application.h"

using namespace std;
using namespace boost;
using namespace lost::application;

int main(int argn, char** args)
{
  try
  {
    static ApplicationPtr app = Application::create(argn, args);
//    app->runLoopWaitsForEvents(true);
    app->run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}


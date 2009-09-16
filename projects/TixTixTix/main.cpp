#include "lost/application/Application.h"

using namespace std;
using namespace lost::common;
using namespace lost::application;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    // run
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

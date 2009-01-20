#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/event/Receive.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::application;
using namespace lost::event;

int main(int argn, char** args)
{
  try
  {
    Application app;
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}


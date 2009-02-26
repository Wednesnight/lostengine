#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"
#include "Controller.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::common;
using namespace lost::resource;
using namespace lost::application;
using namespace lost::event;

int main( int argc, char *argv[] )
{
  try
  {
    Controller controller;
    controller.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}

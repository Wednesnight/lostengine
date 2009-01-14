#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/event/Receive.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::application;
using namespace lost::event;

bool running;

void quitEventHandler(boost::shared_ptr<Event> event)
{
  running = false;
}

void mainLoop()
{
  DOUT("starting mainLoop");
  while(running)
  {
  }
  DOUT("stopping mainLoop");
}

int main(int argn, char** args)
{
  running = true;

  try
  {
    Application app;
    app.mainLoop = mainLoop;
    app.addEventListener(lost::application::ApplicationEvent::QUIT(), quitEventHandler);
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

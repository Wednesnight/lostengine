#include <iostream>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"

using namespace std;
using namespace lost;
using namespace lost::math;
using namespace lost::event;
using namespace lost::application;

ApplicationPtr app;
WindowPtr window;

void update(ApplicationPtr app)
{
    DOUT("update");
}


void keyHandler(KeyEventPtr event)
{
}

int main(int argn, char** args)
{
  try
  {
    DOUT("asd");
  app = Application::create(update);
//  app->runLoopWaitsForEvents(true);
  app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
  window = app->createWindow("window", WindowParams("Filt3rz", Rect(50,200,640,480)));
  window->context->makeCurrent();
  app->run();      
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}


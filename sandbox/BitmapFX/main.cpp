#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/Timer.h"
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
  LogLevel( log_all );

  try
  {
    Application app;
    Controller  controller;
    Timer       redrawTimer("redraw", 1.0/60.0); 

    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), bind(&Controller::init, &controller, _1));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}

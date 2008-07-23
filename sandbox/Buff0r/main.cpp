#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ResizeEvent.h"

#include "Buff0r.h"
#include <boost/bind.hpp>

using namespace std;
using namespace lost::common;
using namespace lost::application;
using namespace boost;
using namespace lost::event;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;    
    Buff0r buff0r(app.displayAttributes, app.loader);
    app.addEventListener(ApplicationEvent::INIT(), bind(&Buff0r::init, &buff0r, _1));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Buff0r::keyboard, &buff0r, _1)));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Buff0r::keyboard, &buff0r, _1)));
    app.addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Buff0r::resetViewPort, &buff0r, _1)));
    buff0r.quit.connect(bind(&Application::quit, &app));

    Timer redrawTimer("redrawTimer", 1.0/60.0);
    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Buff0r::redraw, &buff0r, _1)));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

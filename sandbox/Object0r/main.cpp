#include "Object0r.h"

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Object0r    object0r;
    
    app.addEventListener(ApplicationEvent::INIT(), receive<Event>(bind(&Object0r::init, &object0r, _1)));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Object0r::keyHandler, &object0r, _1)));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Object0r::keyHandler, &object0r, _1)));
    app.addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Object0r::resizeHandler, &object0r, _1)));
    object0r.quit.connect(bind(&Application::quit, &app));

    Timer renderTimer("render", 0.015);
    renderTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Object0r::render, &object0r, _1)));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;

void mouseButton(shared_ptr<MouseEvent> event)
{
  DOUT("mb stuff");
}

void mouseMove(shared_ptr<MouseEvent> event)
{
  DOUT("mm stuff");
}

void keyHandler(shared_ptr<KeyEvent> event)
{
  DOUT("kb stuff");
}

void resize(shared_ptr<ResizeEvent> event)
{
  DOUT("resize w: "<<event->width << " h:" << event->height);
}

void timerAction(shared_ptr<TimerEvent> event)
{
  DOUT("Timer "<<event->timer->name()<<" called after "<<event->passedSec << " sec");
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    app.addEventListener(MouseEvent::MOUSE_UP(), receive<MouseEvent>(mouseButton));
    app.addEventListener(MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(mouseButton));
    app.addEventListener(MouseEvent::MOUSE_MOVE(), receive<MouseEvent>(mouseMove));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(keyHandler));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
    app.addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(resize));
    
    Timer t1("fuggn", 0.020);t1.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(timerAction));
    Timer t2("orkor", 0.200);t2.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(timerAction));
    
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}
#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"

using namespace std;
using namespace lost::common;
using namespace lost::application;

void mouseButton(const lost::event::MouseEvent& event)
{
  DOUT("mb stuff");
}

void mouseMove(const lost::event::MouseEvent& event)
{
  DOUT("mm stuff");
}

void keyHandler(const lost::event::KeyEvent&)
{
  DOUT("kb stuff");
}

void resize(int width, int height)
{
  DOUT("resize w: "<<width << " h:" << height);
}

void timerAction(double passed, Timer* timer)
{
  DOUT("Timer "<<timer->name()<<" called after "<<passed << " sec");
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    app.mouseButton.connect(boost::function<void(const lost::event::MouseEvent& event)>(mouseButton));
    app.mouseMove.connect(boost::function<void(const lost::event::MouseEvent& event)>(mouseMove));
    app.key.connect(boost::function<void(const lost::event::KeyEvent& event)>(keyHandler));
    app.windowResize.connect(boost::function<void(int, int)>(resize));
    
    Timer t1("fuggn", 0.020);
    t1.action.connect(boost::function<void(double passed, Timer* timer)>(timerAction));
    Timer t2("orkor", 0.200);
    t2.action.connect(boost::function<void(double passed, Timer* timer)>(timerAction));
    
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}
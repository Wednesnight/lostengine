#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "Buff0r.h"
#include <boost/bind.hpp>

using namespace std;
using namespace lost::common;
using namespace lost::application;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    if(GLEE_EXT_framebuffer_object)
    {  DOUT("Framebuffer objects OK");}
    else
      DOUT("NO framebuffer objects");
    
    Buff0r buff0r(app.displayAttributes);
    app.key.connect(boost::bind(&Buff0r::keyboard, &buff0r, _1));
    app.windowResize.connect(boost::bind(&Buff0r::resetViewPort, &buff0r, _1,_2));
    buff0r.quit.connect(boost::bind(&Application::quit, &app));
    Timer t1("redrawTimer", 0.015);
    t1.action.connect(boost::bind(&Buff0r::redraw, &buff0r, _1, _2));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

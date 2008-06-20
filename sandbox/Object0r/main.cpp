#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "Object0r.h"
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

    Object0r object0r(app.displayAttributes);
    app.key.connect(boost::bind(&Object0r::keyboard, &object0r, _1));
    app.windowResize.connect(boost::bind(&Object0r::resetViewPort, &object0r, _1,_2));
    object0r.quit.connect(boost::bind(&Application::quit, &app));
    Timer t1("redrawTimer", 0.015);
    t1.action.connect(boost::bind(&Object0r::redraw, &object0r, _1, _2));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

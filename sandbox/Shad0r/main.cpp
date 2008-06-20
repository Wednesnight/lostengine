#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "Shad0r.h"
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
    // at this point, the OpenGL context exists and teh window is open
    // make sure you don't call any OpenGL functions before this point
    // this is also true for any GLee extension checks
    DOUT("----------------------------------------------------");
    DOUT("checking OpenGL capabilities");
    if(GLEE_VERSION_1_2) { DOUT("1.2 OK");} else DOUT("NO 1.2 ");
    if(GLEE_ARB_imaging) { DOUT("imaging OK");} else DOUT("NO imaging ");
    if(GLEE_VERSION_1_3) { DOUT("1.3 OK");}else DOUT("NO 1.3 ");
    if(GLEE_VERSION_1_4) { DOUT("1.4 OK");}else DOUT("NO 1.4 ");
    if(GLEE_VERSION_1_5) { DOUT("1.5 OK");}else DOUT("NO 1.5 ");
    if(GLEE_VERSION_2_0) {DOUT("2.0 OK");}else DOUT("NO 2.0 ");
    if(GLEE_VERSION_2_1) {DOUT("2.1 OK");}else DOUT("NO 2.1 ");
    DOUT("----------------------------------------------------");

    Shad0r shad0r(app.displayAttributes);
    app.key.connect(boost::bind(&Shad0r::keyboard, &shad0r, _1));
    app.windowResize.connect(boost::bind(&Shad0r::resetViewPort, &shad0r, _1,_2));
    shad0r.quit.connect(boost::bind(&Application::quit, &app));
    Timer t1("redrawTimer", 0.015);
    t1.action.connect(boost::bind(&Shad0r::redraw, &shad0r, _1, _2));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

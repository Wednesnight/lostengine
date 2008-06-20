#include <boost/bind.hpp>

#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "FunkyFractals.h"

using namespace std;
using namespace lost::application;
using namespace lost::common;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;

    FunkyFractals funkyfractals(app.displayAttributes);
    app.key.connect(boost::bind(&FunkyFractals::keyboard, &funkyfractals, _1));
    app.windowResize.connect(boost::bind(&FunkyFractals::resetViewPort, &funkyfractals, _1,_2));
    funkyfractals.quit.connect(boost::bind(&Application::quit, &app));
    Timer t1("redrawTimer", 0.015);
    t1.action.connect(boost::bind(&FunkyFractals::redraw, &funkyfractals, _1, _2));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}

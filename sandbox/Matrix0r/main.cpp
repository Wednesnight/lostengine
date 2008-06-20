#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "Matrix0r.h"
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
    Matrix0r matrix0r(app.displayAttributes);
    app.key.connect(boost::bind(&Matrix0r::keyboard, &matrix0r, _1));
    app.windowResize.connect(boost::bind(&Matrix0r::resetViewPort, &matrix0r, _1,_2));
    matrix0r.quit.connect(boost::bind(&Application::quit, &app));
    Timer t1("redrawTimer", 0.015);
    t1.action.connect(boost::bind(&Matrix0r::redraw, &matrix0r, _1, _2));

    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

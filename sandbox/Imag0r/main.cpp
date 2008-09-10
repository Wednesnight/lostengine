#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include <boost/bind.hpp>
#include "Controller.h"

using namespace std;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;
using namespace lost::platform;
using namespace lost::bitmap;
using namespace lost::resource;
using namespace lost::application;
using namespace boost;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Controller controller(appInstance->loader);

    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&Controller::init, &controller, _1)));
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}

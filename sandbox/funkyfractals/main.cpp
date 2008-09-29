#include <boost/bind.hpp>

#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/KeyEvent.h"

#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/lua/bindings/LostLSystemLSystem.h"
#include "lost/lua/bindings/LostMathVec3.h"

#include "FunkyFractals.h"

using namespace std;
using namespace lost::application;
using namespace lost::common;
using namespace lost::event;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    FunkyFractals funkyfractals;

    appInstance->addEventListener(ApplicationEvent::INIT(), receive<ApplicationEvent>(bind(&FunkyFractals::init, &funkyfractals, _1)));

    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}

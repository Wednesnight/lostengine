#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/event/Receive.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::application;
using namespace lost::event;

void buttonClick(shared_ptr<Event> event)
{
  DOUT("buttonClick");
}

void run(shared_ptr<Event> event)
{
  luabind::object screen = luabind::globals(*appInstance->interpreter)["controls"]["screen"];
  luabind::object ui = screen("ui");
  luabind::object window = ui("window");
  luabind::object button = window("button");
  if (button)
  {
    EventDispatcher* dispatcher = luabind::object_cast<EventDispatcher*>(button);
    dispatcher->addEventListener("BUTTON_CLICK", buttonClick);
  }
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;

    app.addEventListener(ApplicationEvent::RUN(), run);
    
    // run
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

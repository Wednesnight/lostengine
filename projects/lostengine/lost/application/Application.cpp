#include "lost/application/Application.h"

using namespace lost::event;

namespace lost
{
  namespace application
  {

    void Application::dispatchEvent(EventPtr event)
    {
      EventDispatcher::dispatchEvent(event);
      if (screen)
      {
        screen->dispatchEvent(event);
      }
    }

  }
}

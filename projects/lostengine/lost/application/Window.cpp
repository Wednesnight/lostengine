#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Window.h"
#include "lost/application/TaskletConfig.h"

namespace lost
{
  namespace application
  {
    
    Window::Window(const event::EventDispatcherPtr& inDispatcher,
                   TaskletConfig* inConfig)
    : dispatcher(inDispatcher),
      config(inConfig)
    {
      // platform specific initialization
      initialize();
      size = math::Vec2(config->windowRect.width, config->windowRect.height);
    }

    Window::~Window()
    {
      finalize();
    }

  }
}

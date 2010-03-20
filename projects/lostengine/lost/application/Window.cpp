#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/Window.h"

namespace lost
{
  namespace application
  {
    
    Window::Window(const event::EventDispatcherPtr& inDispatcher,
                   const WindowParams& inParams)
    : dispatcher(inDispatcher),
      params(inParams)
    {
      // platform specific initialization
      initialize();
    }

    Window::~Window()
    {
      finalize();
    }

  }
}

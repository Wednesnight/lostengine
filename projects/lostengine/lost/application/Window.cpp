#include "lost/application/Window.h"

namespace lost
{
  namespace application
  {
    
    Window::Window(const lost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
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

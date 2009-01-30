#include "lost/application/Window.h"

namespace lost
{
  namespace application
  {
    
    Window::Window(const boost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
                   const WindowParams& inParams)
    : dispatcher(inDispatcher),
      params(inParams)
    {
      initialize();
    }

    boost::shared_ptr<Window> Window::create(const boost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
                                             const WindowParams& inParams)
    {
      return boost::shared_ptr<Window>(new Window(inDispatcher, inParams));
    }

    Window::~Window()
    {
      finalize();
    }

  }
}

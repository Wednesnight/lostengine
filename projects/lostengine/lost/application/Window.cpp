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
      // platform specific initialization
      initialize();
      
      canvas.reset(new gl::Canvas(context, lost::math::Rect(0, 0, params.rect.width, params.rect.height)));
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

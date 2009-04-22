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
      
      canvas.reset(new gl::Canvas(context, lost::math::Rect(0, 0, params.rect.width, params.rect.height)));
    }

    lost::shared_ptr<Window> Window::create(const lost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
                                             const WindowParams& inParams)
    {
      return lost::shared_ptr<Window>(new Window(inDispatcher, inParams));
    }

    Window::~Window()
    {
      finalize();
    }

  }
}

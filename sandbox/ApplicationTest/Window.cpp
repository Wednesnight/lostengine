#include "Window.h"

Window::Window(const boost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
               const WindowParams& inParams)
: dispatcher(inDispatcher),
  params(inParams)
{
  initialize();
}

Window::~Window()
{
  finalize();
}

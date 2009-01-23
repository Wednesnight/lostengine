#include "Window.h"

Window::Window(const WindowParams& inParams)
: params(inParams)
{
  initialize();
}

Window::~Window()
{
  finalize();
}

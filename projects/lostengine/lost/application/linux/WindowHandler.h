#ifndef LOST_APPLICATION_LINUX_WINDOWHANDLER_H
#define LOST_APPLICATION_LINUX_WINDOWHANDLER_H

#include "lost/application/Window.h"
#include <X11/Xlib.h>
#include <string>

namespace lost
{
  namespace application
  {
    namespace linux_
    {
      struct WindowHandler
      {
      private:
	bool running;

	Window* window;
	Display* display;
	::Window nativeWindow;

	Atom WM_DELETE_WINDOW;
	Atom WM_WAKEUP;
	Atom XdndDrop;

      public:
	WindowHandler(Window* window, Display* display, ::Window nativeWindow);
	void operator()();
	bool wakeupAndFinish();
      };
    }  
  }
}

#endif

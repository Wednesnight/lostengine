#ifndef LOST_APPLICATION_LINUX_WINDOWHANDLER_H
#define LOST_APPLICATION_LINUX_WINDOWHANDLER_H

#include "lost/application/Window.h"
#include "lost/application/KeyCode.h"
#include "lost/event/Event.h"
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
        struct Property
        {
          unsigned char *data;
          int format, nitems;
          Atom type;
        };

        bool running;

        Window* window;
        Display* display;
        ::Window nativeWindow;

        Atom WM_DELETE_WINDOW;
        Atom WM_WAKEUP;

        Atom XdndTypeList;
        Atom XdndStatus;
        Atom XdndActionCopy;
        Atom XdndActionMove;
        Atom XdndActionLink;
        Atom XdndActionList;
        Atom XdndSelection;
        Atom XdndEnter;
        Atom XdndPosition;
        Atom XdndDrop;
        Atom XdndLeave;
        Atom XdndFinished;

        // events
        KeyCode translateKeyCode(int nativeKeyCode);
        void dispatchKeyEvent(int code, char* character, bool pressed, bool repeat);
        void dispatchMouseEvent(event::Type which, int x, int y, int absX, int absY, int buttonCode, bool pressed);
        void dispatchDragNDropEvent(const string& filename, int x, int y, int absX, int absY);
        
        // atom handling
        string GetAtomName(Atom a);
        Property read_property(::Window w, Atom property);

        // Xdnd
        Atom dragAccept(XEvent& event);
        
      public:
        WindowHandler(Window* window, Display* display, ::Window nativeWindow);
        void operator()();
        bool wakeupAndFinish();
      };

    }  
  }
}

#endif

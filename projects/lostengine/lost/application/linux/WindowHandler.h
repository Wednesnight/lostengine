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

        Display* display;
        ::Window xWindow;
        Window* window;

        Atom WM_DELETE_WINDOW;

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

        KeySym key;
        char text[255];

        int currentButton;
        Atom XdndFormat;
        int XdndVersion;
        Atom sel;

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
        WindowHandler(Display* display, ::Window xWindow, Window* window);
        void handleEvent(XEvent& event);
      };

    }  
  }
}

#endif

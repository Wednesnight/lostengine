/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

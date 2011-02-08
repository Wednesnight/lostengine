#include "lost/application/linux/WindowHandler.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/application/MouseButton.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/TaskletConfig.h"
#include "lost/common/Logger.h"

#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>

namespace lost
{
  namespace application
  {
    namespace linux_
    {

      WindowHandler::WindowHandler (Window* window, Display* display, ::Window nativeWindow)
      : window (window),
        display (display),
        nativeWindow (nativeWindow)
      {
        WM_DELETE_WINDOW = XInternAtom (display, "WM_DELETE_WINDOW", True);
        WM_WAKEUP = XInternAtom (display, "WM_WAKEUP", True);

        XdndTypeList = XInternAtom(display, "XdndTypeList", False);
        XdndStatus = XInternAtom(display, "XdndStatus", False);
        XdndActionCopy = XInternAtom(display, "XdndActionCopy", False);
        XdndActionMove = XInternAtom(display, "XdndActionMove", False);
        XdndActionLink = XInternAtom(display, "XdndActionLink", False);
        XdndActionList = XInternAtom(display, "XdndActionList", False);
        XdndSelection = XInternAtom(display, "XdndSelection", False);
        XdndEnter = XInternAtom (display, "XdndEnter", False);
        XdndPosition = XInternAtom (display, "XdndPosition", False);
        XdndDrop = XInternAtom (display, "XdndDrop", False);
        XdndLeave = XInternAtom (display, "XdndLeave", False);
        XdndFinished = XInternAtom (display, "XdndFinished", False);
      }

      void WindowHandler::operator() ()
      {
        running = true;
        XEvent event;
        KeySym key;
        char text[255];
        int currentButton = 0;

        Atom XdndFormat = None;
        int XdndVersion = 0;
        Atom sel = XInternAtom(display, "PRIMARY", False);
        
        while (running)
        {
          XNextEvent (display, &event);
          switch (event.type)
          {

            case Expose:
              break;

            case ConfigureNotify:
              if ((event.xconfigure.width != window->config->windowRect.width) || (event.xconfigure.height != window->config->windowRect.height))
              {
                  window->dispatcher->queueEvent (ResizeEvent::create (event.xconfigure.width, event.xconfigure.height));
              }
              break;

            case ButtonPress:
              currentButton = event.xbutton.button;
            case ButtonRelease:
              currentButton = 0;
              dispatchMouseEvent(event.type == ButtonPress ? MouseEvent::MOUSE_DOWN() : MouseEvent::MOUSE_UP(), event.xbutton.x, event.xbutton.y,
                                 event.xbutton.x_root, event.xbutton.y_root, event.xbutton.button, event.type == ButtonPress);
              break;

            case KeyPress:
            case KeyRelease:
              XLookupString (&event.xkey, text, 255, &key, 0);
              dispatchKeyEvent(key, text, event.type == KeyPress, false);
              break;

            case MotionNotify:
              dispatchMouseEvent(MouseEvent::MOUSE_MOVE(), event.xmotion.x, event.xmotion.y,
                                 event.xmotion.x_root, event.xmotion.y_root, currentButton, currentButton != 0);
              break;

            case ClientMessage:
              DOUT("ClientMessage: " << GetAtomName(event.xclient.message_type) << " (" << event.xclient.format << ")");
              if (event.xclient.data.l[0] == WM_DELETE_WINDOW) {
                window->dispatcher->queueEvent (WindowEvent::create (WindowEvent::CLOSE(), window));
              }
              else if (event.xclient.data.l[0] == WM_WAKEUP) {
                break;
              }
              else if (event.xclient.message_type == XdndEnter) {
                XdndFormat = dragAccept(event);
                XdndVersion = event.xclient.data.l[1] >> 24;
              }
              else if (event.xclient.message_type == XdndPosition) {
                if (XdndFormat != None) {
                  XClientMessageEvent m;
                  memset(&m, sizeof(m), 0);
                  m.type = ClientMessage;
                  m.display = event.xclient.display;
                  m.window = event.xclient.data.l[0];
                  m.message_type = XdndStatus;
                  m.format = 32;
                  m.data.l[0] = nativeWindow;
                  m.data.l[1] = True;
                  m.data.l[2] = 0; // empty rectangle
                  m.data.l[3] = 0;
                  m.data.l[4] = XdndActionCopy; // accept copy
                  
                  XSendEvent(display, event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                  XFlush(display);
                }
              }
              else if (event.xclient.message_type == XdndDrop) {
                if (XdndFormat == None) {
                  // It's sending anyway, despite instructions to the contrary.
                  // So reply that we're not interested.
                  XClientMessageEvent m;
                  memset(&m, sizeof(m), 0);
                  m.type = ClientMessage;
                  m.display = event.xclient.display;
                  m.window = event.xclient.data.l[0];
                  m.message_type = XdndFinished;
                  m.format = 32;
                  m.data.l[0] = nativeWindow;
                  m.data.l[1] = False;
                  m.data.l[2] = None; // failed

                  XSendEvent(display, event.xclient.data.l[0], False, NoEventMask, (XEvent*)&m);
                  XFlush(display);
                }
                else
                {
                  if (XdndVersion >= 1) {
                    XConvertSelection(display, XdndSelection, XdndFormat, sel, nativeWindow, event.xclient.data.l[2]);
                  }
                  else {
                    XConvertSelection(display, XdndSelection, XdndFormat, sel, nativeWindow, CurrentTime);
                  }
                }
              }
              else if (event.xclient.message_type == XdndLeave) {
                XdndFormat = None; // cancelled
              }
              break;

            case SelectionNotify:
            {
              XClientMessageEvent m;
              memset(&m, sizeof(m), 0);
              m.type = ClientMessage;
              m.display = display;
              m.window = event.xselection.requestor;
              m.message_type = XdndFinished;
              m.format = 32;
              m.data.l[0] = nativeWindow;
              if (event.xselection.target != None && event.xselection.target == XdndFormat) {
                Property prop = read_property(nativeWindow, sel);
                std::ostringstream os;
                os.write((char*)prop.data, prop.nitems * prop.format / 8);
                XFree(prop.data);

                // success
                m.data.l[1] = True;
                m.data.l[2] = XdndActionCopy;

                DOUT("Received drop: " << os.str());
                ::Window rootWindow, childWindow;
                int x, y, absX, absY;
                unsigned int mask;
                XQueryPointer(display, nativeWindow, &rootWindow, &childWindow, &absX, &absY, &x, &y, &mask);
                dispatchDragNDropEvent(os.str(), x, y, absX, absY);
              } else {
                // fail
                m.data.l[1] = False;
                m.data.l[2] = None;
              }
              XSendEvent(display, event.xselection.requestor, False, NoEventMask, (XEvent*)&m);
              XSync(display, False);
              break;
            }

            default:
              break;
          }
        }
      }

      bool WindowHandler::wakeupAndFinish()
      {
          running = false;

          XEvent event;
          event.xclient.message_type = ClientMessage;
          event.xclient.data.l[0] = WM_WAKEUP;
          return XSendEvent (display, nativeWindow, False, 0l, &event);
      }

      KeyCode WindowHandler::translateKeyCode(int nativeKeyCode)
      {
        switch (nativeKeyCode)
        {
          case 65 : return lost::application::K_A;
          case 66 : return lost::application::K_B;
          case 67 : return lost::application::K_C;
          case 68 : return lost::application::K_D;
          case 69 : return lost::application::K_E;
          case 70 : return lost::application::K_F;
          case 71 : return lost::application::K_G;
          case 72 : return lost::application::K_H;
          case 73 : return lost::application::K_I;
          case 74 : return lost::application::K_J;
          case 75 : return lost::application::K_K;
          case 76 : return lost::application::K_L;
          case 77 : return lost::application::K_M;
          case 78 : return lost::application::K_N;
          case 79 : return lost::application::K_O;
          case 80 : return lost::application::K_P;
          case 81 : return lost::application::K_Q;
          case 82 : return lost::application::K_R;
          case 83 : return lost::application::K_S;
          case 84 : return lost::application::K_T;
          case 85 : return lost::application::K_U;
          case 86 : return lost::application::K_V;
          case 87 : return lost::application::K_W;
          case 88 : return lost::application::K_X;
          case 89 : return lost::application::K_Y;
          case 90 : return lost::application::K_Z;

          case 48 : return lost::application::K_0;
          case 49 : return lost::application::K_1;
          case 50 : return lost::application::K_2;
          case 51 : return lost::application::K_3;
          case 52 : return lost::application::K_4;
          case 53 : return lost::application::K_5;
          case 54 : return lost::application::K_6;
          case 55 : return lost::application::K_7;
          case 56 : return lost::application::K_8;
          case 57 : return lost::application::K_9;

          case XK_dead_circumflex : return lost::application::K_CIRCUMFLEX;
          case XK_Help : return lost::application::K_HELP;
          case XK_apostrophe : return lost::application::K_APOSTROPHE;

          case XK_Return : return lost::application::K_ENTER;
          case XK_Tab : return lost::application::K_TAB;
          case XK_space : return lost::application::K_SPACE;
          case XK_BackSpace : return lost::application::K_BACKSPACE;
          case XK_Escape : return lost::application::K_ESCAPE;

          case XK_Left : return lost::application::K_LEFT;
          case XK_Right : return lost::application::K_RIGHT;
          case XK_Up : return lost::application::K_UP;
          case XK_Down : return lost::application::K_DOWN;
          case XK_Page_Up : return lost::application::K_PAGEUP;
          case XK_Page_Down : return lost::application::K_PAGEDOWN;
          case XK_Home : return lost::application::K_HOME;
          case XK_End : return lost::application::K_END;

          case XK_F1 : return lost::application::K_F1;
          case XK_F2 : return lost::application::K_F2;
          case XK_F3 : return lost::application::K_F3;
          case XK_F4 : return lost::application::K_F4;
          case XK_F5 : return lost::application::K_F5;
          case XK_F6 : return lost::application::K_F6;
          case XK_F7 : return lost::application::K_F7;
          case XK_F8 : return lost::application::K_F8;
          case XK_F9 : return lost::application::K_F9;
          case XK_F10 : return lost::application::K_F10;
          case XK_F11 : return lost::application::K_F11;
          case XK_F12 : return lost::application::K_F12;

          default : return lost::application::K_UNKNOWN;
        }
      }

      void WindowHandler::dispatchKeyEvent (int code, char* character, bool pressed, bool repeat)
      {
        KeyEventPtr event (new KeyEvent (pressed ? KeyEvent::KEY_DOWN() : KeyEvent::KEY_UP()));
        event->window    = window;
        event->key       = translateKeyCode (code);
        event->character = character;
        event->pressed   = pressed;
        event->repeat    = repeat;
        window->dispatcher->queueEvent (event);
      }

      void WindowHandler::dispatchMouseEvent (event::Type which, int x, int y, int absX, int absY, int buttonCode, bool pressed)
      {
        MouseEventPtr event (new MouseEvent (which));
        event->window  = window;
        event->pos     = math::Vec2 (x, y);
        event->absPos  = math::Vec2 ((float) absX, absY);
        MouseButton button = MB_UNKNOWN;
        if (buttonCode == Button1) {
          button = MB_LEFT;
        }
        else if (buttonCode == Button2) {
          button = MB_MIDDLE;
        }
        else if (buttonCode == Button3) {
          button = MB_RIGHT;
        }
        event->button  = button;
        event->pressed = pressed;
        //event->scrollDelta = lost::math::Vec3([event deltaX], [event deltaY], [event deltaZ]);
        window->dispatcher->queueEvent (event);
      }

      void WindowHandler::dispatchDragNDropEvent(const std::string& files, int x, int y, int absX, int absY)
      {
        std::vector<std::string> fileList;
        boost::split(fileList, files, boost::is_any_of("\r\n"), boost::token_compress_on);
        if (fileList.size() > 0) {
          // FIXME: we only support single files at the moment
          std::string filename = fileList.front();
          if (filename.find("file://") != std::string::npos) {
            filename = filename.substr(filename.find_first_not_of("file://")-1);
          }
          DOUT("Dropped file: " << filename);
          DragNDropEventPtr dragNDropEvent(new DragNDropEvent(DragNDropEvent::DROP(), filename));
          dragNDropEvent->window  = window;
          dragNDropEvent->pos     = lost::math::Vec2((float)x, (float)y);
          dragNDropEvent->absPos  = lost::math::Vec2((float)absX, (float)absY);
          window->dispatcher->queueEvent(dragNDropEvent);
        }
      }

      Atom WindowHandler::dragAccept(XEvent& event)
      {
        Atom result = None;

        int version = event.xclient.data.l[1] >> 24;
        DOUT("Xdnd Version: " << version);

        ::Window source = event.xclient.data.l[0];
        DOUT(std::hex << "Source window = 0x" << source << std::dec);
        
        Property p = read_property(source , XdndTypeList);
        for (int i = 0; !result && i < p.nitems; i++)
        {
          Atom atom = ((Atom*)p.data)[i];
          std::string atom_name = GetAtomName(atom);
          DOUT("Type " << i << " = " << atom_name);

          if (atom_name == "text/uri-list") {
            DOUT("Will request type: " << atom_name);
            result = atom;
          }
        }
        XFree(p.data);

        return result;
      }

      //Convert an atom name in to a std::string
      std::string WindowHandler::GetAtomName(Atom a)
      {
        if(a == None)
          return "None";
        else
          return XGetAtomName(display, a);
      }

      //This fetches all the data from a property
      WindowHandler::Property WindowHandler::read_property(::Window w, Atom property)
      {
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *ret=0;

        int read_bytes = 1024;

        //Keep trying to read the property until there are no
        //bytes unread.
        do
        {
          if(ret != 0)
            XFree(ret);
          XGetWindowProperty(display, w, property, 0, read_bytes, False, AnyPropertyType,
                            &actual_type, &actual_format, &nitems, &bytes_after,
                            &ret);

          read_bytes *= 2;
        }while(bytes_after != 0);

        DOUT("Actual type: " << GetAtomName(actual_type));
        DOUT("Actual format: " << actual_format);
        DOUT("Number of items: " << nitems);

        Property p = {ret, actual_format, nitems, actual_type};

        return p;
      }

    }
  }
}

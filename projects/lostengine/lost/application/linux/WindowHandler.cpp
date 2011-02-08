#include "lost/application/linux/WindowHandler.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/ProcessEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseButton.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/TaskletConfig.h"

#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <GL/glx.h>

#include <boost/thread.hpp>

namespace lost
{
  namespace application
  {
    namespace linux_
    {
      KeyCode translateKeyCode(int nativeKeyCode);
      void dispatchKeyEvent(Window* window, int code, char* character, bool pressed, bool repeat);
      void dispatchMouseEvent(Window* window, event::Type which, int x, int y, int absX, int absY, int buttonCode, bool pressed);

      WindowHandler::WindowHandler(Window* window, Display* display, ::Window nativeWindow)
      : window(window),
	display(display),
	nativeWindow(nativeWindow)
      {
	WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", True);
	WM_WAKEUP = XInternAtom(display, "WM_WAKEUP", True);
	XdndDrop = XInternAtom(display, "XdndDrop", False);
      }

      void WindowHandler::operator()()
      {
	running = true;
	XEvent event;
	KeySym key;
	char text[255];
	int currentButton = 0;

	while (running) {

	  XNextEvent(display, &event);
	  switch (event.type) {

	    case Expose:
	      break;

	    case ConfigureNotify:
	      if ((event.xconfigure.width != window->config->windowRect.width) || (event.xconfigure.height != window->config->windowRect.height)) {
		window->dispatcher->queueEvent(ResizeEvent::create(event.xconfigure.width, event.xconfigure.height));
	      }
	      break;

	    case ButtonPress:
	      currentButton = event.xbutton.button;
	    case ButtonRelease:
	      currentButton = 0;
	      dispatchMouseEvent(window, event.type == ButtonPress ? MouseEvent::MOUSE_DOWN() : MouseEvent::MOUSE_UP(), event.xbutton.x, event.xbutton.y,
				  event.xbutton.x_root, event.xbutton.y_root, event.xbutton.button, event.type == ButtonPress);
	      break;

	    case KeyPress:
	    case KeyRelease:
	      XLookupString(&event.xkey, text, 255, &key, 0);
	      dispatchKeyEvent(window, key, text, event.type == KeyPress, false);
	      break;

	    case MotionNotify:
	      dispatchMouseEvent(window, MouseEvent::MOUSE_MOVE(), event.xmotion.x, event.xmotion.y,
				  event.xmotion.x_root, event.xmotion.y_root, currentButton, currentButton != 0);
	      break;

	    case ClientMessage:
	      if (event.xclient.data.l[0] == WM_DELETE_WINDOW) {
		window->dispatcher->queueEvent(WindowEvent::create(WindowEvent::CLOSE(), window));
	      }
	      else if (event.xclient.data.l[0] == WM_WAKEUP) {
		break;
	      }
	      else if (event.xclient.data.l[0] == XdndDrop) {
		// TODO
	      }
	      break;

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
	return XSendEvent(display, nativeWindow, False, 0l, &event);
      }

      KeyCode translateKeyCode(int nativeKeyCode)
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

      void dispatchKeyEvent(Window* window, int code, char* character, bool pressed, bool repeat)
      {
	KeyEventPtr event(new KeyEvent(pressed ? KeyEvent::KEY_DOWN() : KeyEvent::KEY_UP()));
	event->window    = window;
	event->key       = translateKeyCode(code);
	event->character = character;
	event->pressed   = pressed;
	event->repeat    = repeat;
	window->dispatcher->queueEvent(event);
      }

      void dispatchMouseEvent(Window* window, event::Type which, int x, int y, int absX, int absY, int buttonCode, bool pressed)
      {
	MouseEventPtr event(new MouseEvent(which));
	event->window  = window;
	event->pos     = math::Vec2(x, y);
	event->absPos  = math::Vec2((float) absX, absY);
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
//	  event->scrollDelta = lost::math::Vec3([event deltaX], [event deltaY], [event deltaZ]);
	window->dispatcher->queueEvent(event);
      }

    }
  }
}

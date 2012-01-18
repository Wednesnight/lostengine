#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/linux/WindowHandler.h"

#include <map>
#include <iostream>
#include <stdexcept>

#include <X11/Xlib.h>

namespace lost
{
  namespace application
  {

    Display* currentXDisplay;

    std::map< ::Window, linux_::WindowHandler* > xWindows;

    void registerXWindow(::Window xWindow, Window* window) {
      if (xWindows.find(xWindow) == xWindows.end()) {
        xWindows[xWindow] = new linux_::WindowHandler(currentXDisplay, xWindow, window);
      }
    }

    void unregisterXWindow(::Window xWindow) {
      if (xWindows.find(xWindow) == xWindows.end()) {
        linux_::WindowHandler* handler = xWindows[xWindow];
        xWindows.erase(xWindow);
        delete handler;
      }
    }

    int handleXError(Display* display, XErrorEvent* event)
    {
      if (event) {
        char buffer[1024];
        XGetErrorText(display, event->error_code, buffer, 1024);
        EOUT("X11: " << buffer);
      }
    }

    struct Application::ApplicationHiddenMembers
    {
      Display *display;
      Atom WM_WAKEUP;
    };

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      XInitThreads();
      XSetErrorHandler(&handleXError);

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;
      if(!(hiddenMembers->display = XOpenDisplay(NULL))) {
        throw std::runtime_error("could not open display");
      }
      hiddenMembers->WM_WAKEUP = XInternAtom(hiddenMembers->display, "WM_WAKEUP", False);

      currentXDisplay = hiddenMembers->display;
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");

      XFree((void *) hiddenMembers->WM_WAKEUP);
      XCloseDisplay(hiddenMembers->display);
      delete hiddenMembers;
    }

    void Application::run(int argc, char *argv[])
    {
      DOUT("Application::run()");

      ApplicationEventPtr applicationEvent = ApplicationEvent::create(ApplicationEvent::RUN());
      eventDispatcher->dispatchEvent(applicationEvent);

      {
        XEvent event;
        event.xclient.message_type = ClientMessage;
        event.xclient.data.l[0] = hiddenMembers->WM_WAKEUP;
        XSendEvent(hiddenMembers->display, 0, False, 0l, &event);
      }

      XEvent event;
      while(running) {
        XFlush(hiddenMembers->display);
        while (XPending(hiddenMembers->display)) {
          XNextEvent(hiddenMembers->display, &event);
          switch (event.type) {

            case ClientMessage:
              if (event.xclient.data.l[0] == hiddenMembers->WM_WAKEUP) {
                DOUT("wakeup!");
                break;
              }

            default:
              if (event.xclient.window) {
                linux_::WindowHandler* windowHandler = NULL;
                if (xWindows.find(event.xclient.window) != xWindows.end()) {
                  windowHandler = xWindows[event.xclient.window];
                }

                if (windowHandler != NULL) {
                  windowHandler->handleEvent(event);
                }
              }
              break;

          }
        }
        eventDispatcher->processEvents();
      }
    }

    void Application::shutdown()
    {
      XEvent event;
      event.xclient.message_type = ClientMessage;
      event.xclient.data.l[0] = hiddenMembers->WM_WAKEUP;
      XSendEvent(hiddenMembers->display, 0, False, 0l, &event);
      XFlush(hiddenMembers->display);
    }

    void Application::showMouse(bool visible)
    {
    }

    void Application::processEvents(const ProcessEventPtr& processEvent)
    {
      XEvent event;
      event.xclient.message_type = ClientMessage;
      event.xclient.data.l[0] = hiddenMembers->WM_WAKEUP;
      XSendEvent(hiddenMembers->display, 0, False, 0l, &event);
      XFlush(hiddenMembers->display);
    }

    void Application::taskletSpawn(const SpawnTaskletEventPtr& event)
    {
      addTasklet(new Tasklet(event->loader));
    }

  }
}


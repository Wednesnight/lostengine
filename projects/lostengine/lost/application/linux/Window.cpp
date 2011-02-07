#include "lost/application/Window.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/application/TaskletConfig.h"

#include <map>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace lost
{
  namespace application
  {

    void threadFunc(Window* window, Display* display)
    {
      XEvent event;
      while(true)
      { 
        XNextEvent(display, &event); 
        DOUT("event: " << event.type);
        switch(event.type)
        {
        }
      }
    }

    struct Window::WindowHiddenMembers
    {
      Display* display;
      XVisualInfo* visualInfo;
      GLXContext glContext;
      Colormap colorMap;
      ::Window window;
      boost::thread* thread;
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      int attributes[] = { 
        GLX_RGBA, 
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE,   1, 
        GLX_GREEN_SIZE, 1, 
        GLX_BLUE_SIZE,  1, 
        None
      };

      hiddenMembers->display = XOpenDisplay(0);
      hiddenMembers->visualInfo = glXChooseVisual(hiddenMembers->display,
          DefaultScreen(hiddenMembers->display), attributes);

      hiddenMembers->glContext = glXCreateContext(hiddenMembers->display,
          hiddenMembers->visualInfo, 0, GL_TRUE);

      hiddenMembers->colorMap = XCreateColormap(hiddenMembers->display,
          RootWindow(hiddenMembers->display, hiddenMembers->visualInfo->screen),
          hiddenMembers->visualInfo->visual, AllocNone);

      XSetWindowAttributes swa;
      swa.colormap = hiddenMembers->colorMap;
      swa.border_pixel = 0;
      swa.event_mask = StructureNotifyMask;
      hiddenMembers->window = XCreateWindow(hiddenMembers->display,
          RootWindow(hiddenMembers->display, hiddenMembers->visualInfo->screen), 
          (int)config->windowRect.x, (int)config->windowRect.y,
          (int)config->windowRect.width, (int)config->windowRect.height,
          0, hiddenMembers->visualInfo->depth, InputOutput, hiddenMembers->visualInfo->visual,
          CWBorderPixel | CWColormap | CWEventMask, &swa);

      glXMakeCurrent(hiddenMembers->display, hiddenMembers->window, hiddenMembers->glContext);
      context.reset(new gl::Context);

      hiddenMembers->thread = new boost::thread(boost::bind(&threadFunc, this, hiddenMembers->display));
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");

      hiddenMembers->thread->detach();
      if (hiddenMembers->thread->joinable()) {
        hiddenMembers->thread->join();
      }
      delete hiddenMembers->thread;

      XDestroyWindow(hiddenMembers->display, hiddenMembers->window);
      XFreeColormap(hiddenMembers->display, hiddenMembers->colorMap);
      glXMakeCurrent(hiddenMembers->display, None, NULL);
      glXDestroyContext(hiddenMembers->display, hiddenMembers->glContext);
      XFree(hiddenMembers->visualInfo);
      XCloseDisplay(hiddenMembers->display);

      delete hiddenMembers;
    }

    void Window::open()
    {
      DOUT("Window::open()");
      XMapWindow(hiddenMembers->display, hiddenMembers->window);
    }

    void Window::close()
    {
      DOUT("Window::close()");
      XUnmapWindow(hiddenMembers->display, hiddenMembers->window);
    }

  }
}


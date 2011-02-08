#include "lost/application/Window.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/application/TaskletConfig.h"
#include "lost/application/linux/WindowHandler.h"

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/Xatom.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace lost
{
  namespace application
  {

    struct Window::WindowHiddenMembers
    {
      Display* display;
      XVisualInfo* visualInfo;
      GLXContext glContext;
      Colormap colorMap;
      ::Window window;
      lost::shared_ptr<linux_::WindowHandler> windowHandler;
      lost::shared_ptr<boost::thread> windowThread;
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      int attributesSingle[] = {
        GLX_RGBA,
        GLX_RED_SIZE,   8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE,  8,
        GLX_DEPTH_SIZE, 16,
        None
      };

      int attributesDouble[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE,   8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE,  8,
        GLX_DEPTH_SIZE, 16,
        None
      };

      XInitThreads();
      hiddenMembers->display = XOpenDisplay(0);

      int majorVersion, minorVersion;
      XF86VidModeQueryVersion(hiddenMembers->display, &majorVersion, &minorVersion);
      IOUT("XFree86 Version: " << majorVersion << "." << minorVersion);
      glXQueryVersion(hiddenMembers->display, &majorVersion, &minorVersion);
      IOUT("glX Version: " << majorVersion << "." << minorVersion);

      int defaultScreen = DefaultScreen(hiddenMembers->display);

      hiddenMembers->visualInfo = glXChooseVisual(hiddenMembers->display, defaultScreen, attributesDouble);
      if (hiddenMembers->visualInfo == NULL) {
        hiddenMembers->visualInfo = glXChooseVisual(hiddenMembers->display, defaultScreen, attributesSingle);
        IOUT("Buffer: Single buffering");
      }
      else {
        IOUT("Buffer: Double buffering");
      }

      hiddenMembers->glContext = glXCreateContext(hiddenMembers->display, hiddenMembers->visualInfo, 0, GL_TRUE);

      hiddenMembers->colorMap = XCreateColormap(hiddenMembers->display, RootWindow(hiddenMembers->display,
      hiddenMembers->visualInfo->screen), hiddenMembers->visualInfo->visual, AllocNone);

      XSetWindowAttributes swa;
      swa.colormap = hiddenMembers->colorMap;
      swa.border_pixel = 0;
      swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
            StructureNotifyMask | ButtonMotionMask | PointerMotionMask;
      hiddenMembers->window = XCreateWindow(hiddenMembers->display, RootWindow(hiddenMembers->display,
      hiddenMembers->visualInfo->screen), (int)config->windowRect.x, (int)config->windowRect.y,
          (int)config->windowRect.width, (int)config->windowRect.height, 0, hiddenMembers->visualInfo->depth,
          InputOutput, hiddenMembers->visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);

      Atom WM_DELETE_WINDOW = XInternAtom(hiddenMembers->display, "WM_DELETE_WINDOW", False);
      XSetWMProtocols(hiddenMembers->display, hiddenMembers->window, &WM_DELETE_WINDOW, 1);
      XSetStandardProperties(hiddenMembers->display, hiddenMembers->window, config->windowTitle.c_str(),
      config->windowTitle.c_str(), None, NULL, 0, NULL);

      // enable drag&drop
      Atom XdndAware = XInternAtom(hiddenMembers->display, "XdndAware", False);
      Atom version = 5;
      XChangeProperty(hiddenMembers->display, hiddenMembers->window, XdndAware, XA_ATOM, 32, PropModeReplace, (unsigned char*)&version, 1);

      glXMakeCurrent(hiddenMembers->display, hiddenMembers->window, hiddenMembers->glContext);
      context.reset(new gl::Context);

      if (glXIsDirect(hiddenMembers->display, hiddenMembers->glContext)) { 
        IOUT("Renderer: Hardware");
      }
      else {
        IOUT("Renderer: Software");
      }

      hiddenMembers->windowHandler.reset(new linux_::WindowHandler(this, hiddenMembers->display, hiddenMembers->window));
      hiddenMembers->windowThread.reset(new boost::thread(boost::bind(&linux_::WindowHandler::operator(), hiddenMembers->windowHandler.get())));
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");

      if (!hiddenMembers->windowHandler->wakeupAndFinish()) {
        hiddenMembers->windowThread->detach();
      }
      if (hiddenMembers->windowThread->joinable()) {
        hiddenMembers->windowThread->join();
      }
      hiddenMembers->windowThread.reset();
      hiddenMembers->windowHandler.reset();

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
      XMapRaised(hiddenMembers->display, hiddenMembers->window);
    }

    void Window::close()
    {
      DOUT("Window::close()");
      XUnmapWindow(hiddenMembers->display, hiddenMembers->window);
    }

  }
}

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

#include "lost/application/Window.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include "lost/application/TaskletConfig.h"

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/Xatom.h>

namespace lost
{
  namespace application
  {

    extern Display* currentXDisplay;

    extern void registerXWindow(::Window xWindow, Window* window);
    extern void unregisterXWindow(::Window xWindow);

    struct Window::WindowHiddenMembers
    {
      Display* display;
      XVisualInfo* visualInfo;
      GLXContext glContext;
      Colormap colorMap;
      ::Window window;
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      int attributesSingle[] = {
        GLX_RGBA,
        GLX_RED_SIZE,   1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE,  1,
        GLX_DEPTH_SIZE, 1,
//         GLX_STEREO,
        None
      };

      int attributesDouble[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_RED_SIZE,   1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE,  1,
        GLX_DEPTH_SIZE, 1,
//         GLX_STEREO,
        None
      };

//      hiddenMembers->display = XOpenDisplay(NULL);
      hiddenMembers->display = lost::application::currentXDisplay;

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

      hiddenMembers->colorMap = XCreateColormap(hiddenMembers->display, RootWindow(hiddenMembers->display,
          hiddenMembers->visualInfo->screen), hiddenMembers->visualInfo->visual, AllocNone);

      XSetWindowAttributes swa;
      swa.background_pixel = 0;
      swa.border_pixel = 0;
      swa.colormap = hiddenMembers->colorMap;
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

      hiddenMembers->glContext = glXCreateContext(hiddenMembers->display, hiddenMembers->visualInfo, NULL, True);
      glXMakeCurrent(hiddenMembers->display, hiddenMembers->window, hiddenMembers->glContext);
      context.reset(new gl::Context);

      if (glXIsDirect(hiddenMembers->display, hiddenMembers->glContext)) { 
        IOUT("Renderer: Hardware");
      }
      else {
        IOUT("Renderer: Software");
      }

      context->clearCurrent();

      registerXWindow(hiddenMembers->window, this);
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");

      unregisterXWindow(hiddenMembers->window);

      XDestroyWindow(hiddenMembers->display, hiddenMembers->window);
      XFreeColormap(hiddenMembers->display, hiddenMembers->colorMap);
      glXMakeCurrent(hiddenMembers->display, None, NULL);
      glXDestroyContext(hiddenMembers->display, hiddenMembers->glContext);
      XFree(hiddenMembers->visualInfo);

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

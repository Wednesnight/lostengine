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
#include "lost/gl/gl.h"
#include "lost/application/TaskletConfig.h"
#include "lost/application/ResizeEvent.h"
#include "lost/event/EventDispatcher.h"

extern "C" {
  #include <jni.h>
  #include <android_native_app_glue.h>
  
  extern struct android_app* getAndroidApp();
}

namespace lost
{
  namespace application
  {

    struct Window::WindowHiddenMembers
    {
      ANativeWindow* nativeWindow;
      EGLDisplay display;
      EGLSurface surface;
      EGLContext context;
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      hiddenMembers->nativeWindow = getAndroidApp()->window;
      
      const EGLint attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
      };
      
      EGLint w, h, dummy, format;
      EGLint numConfigs;
      EGLConfig config;
      
      hiddenMembers->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
      
      eglInitialize(hiddenMembers->display, 0, 0);
      
      /* Here, the application chooses the configuration it desires. In this
       * sample, we have a very simplified selection process, where we pick
       * the first EGLConfig that matches our criteria */
      eglChooseConfig(hiddenMembers->display, attribs, &config, 1, &numConfigs);
      
      /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
       * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
       * As soon as we picked a EGLConfig, we can safely reconfigure the
       * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
      eglGetConfigAttrib(hiddenMembers->display, config, EGL_NATIVE_VISUAL_ID, &format);
      
      ANativeWindow_setBuffersGeometry(hiddenMembers->nativeWindow, 0, 0, format);
      
      hiddenMembers->surface = eglCreateWindowSurface(hiddenMembers->display, config, hiddenMembers->nativeWindow, NULL);
      hiddenMembers->context = eglCreateContext(hiddenMembers->display, config, NULL, NULL);
      
      eglMakeCurrent(hiddenMembers->display, hiddenMembers->surface, hiddenMembers->surface, hiddenMembers->context);

      eglQuerySurface(hiddenMembers->display, hiddenMembers->surface, EGL_WIDTH, &w);
      eglQuerySurface(hiddenMembers->display, hiddenMembers->surface, EGL_HEIGHT, &h);

      // FIXME: fake resize event from here, window size won't probably ever change so we're fine
      ResizeEventPtr resizeEvent(new ResizeEvent(w, h));
      dispatcher->queueEvent(resizeEvent);

      context.reset(new gl::Context);
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");

      if (hiddenMembers->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(hiddenMembers->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (hiddenMembers->context != EGL_NO_CONTEXT) {
          eglDestroyContext(hiddenMembers->display, hiddenMembers->context);
        }
        if (hiddenMembers->surface != EGL_NO_SURFACE) {
          eglDestroySurface(hiddenMembers->display, hiddenMembers->surface);
        }
        eglTerminate(hiddenMembers->display);
      }
      
      hiddenMembers->context = EGL_NO_CONTEXT;
      hiddenMembers->surface = EGL_NO_SURFACE;
      hiddenMembers->display = EGL_NO_DISPLAY;
      hiddenMembers->nativeWindow = NULL;
      delete hiddenMembers;
    }

    void Window::open()
    {
      DOUT("Window::open()");
    }

    void Window::close()
    {
      DOUT("Window::close()");
    }

  }
}

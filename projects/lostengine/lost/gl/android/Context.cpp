#include "lost/gl/Context.h"
#include "lost/gl/gl.h"

namespace lost
{
  namespace gl
  {

    // set to the current context in makeCurrent
    static void* currentContext = NULL;

    struct Context::ContextHiddenMembers
    {
      EGLDisplay display;
      EGLSurface drawSurface;
      EGLSurface readSurface;
      EGLContext context;
    };

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->display = eglGetCurrentDisplay();
      hiddenMembers->drawSurface = eglGetCurrentSurface(EGL_DRAW);
      hiddenMembers->readSurface = eglGetCurrentSurface(EGL_READ);
      hiddenMembers->context = eglGetCurrentContext();
      currentContext = hiddenMembers;
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      eglMakeCurrent(hiddenMembers->display, hiddenMembers->drawSurface, hiddenMembers->readSurface, hiddenMembers->context);
      currentContext = hiddenMembers;
    }

    void Context::swapBuffers()
    {
      eglSwapBuffers(hiddenMembers->display, hiddenMembers->drawSurface);
    }

    void* Context::getCurrentOsSpecific()
    {
      return currentContext;
    }

    void Context::setCurrentOsSpecififc(void* ctx)
    {
      Context::ContextHiddenMembers* members = (Context::ContextHiddenMembers*)ctx;
      eglMakeCurrent(members->display, members->drawSurface, members->readSurface, members->context);
    }


    void Context::vsync(bool enable)
    {
      // FIXME: implement Context::vsync
    }

    void Context::multithreaded(bool enable)
    {
      // FIXME: implement Context::multithreaded
    }

  }
}

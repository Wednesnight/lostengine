#include "lost/gl/Context.h"
#include "lost/gl/gl.h"

namespace lost
{
  namespace gl
  {

    // set to the current context in makeCurrent
    void* currentContext;

    struct Context::ContextHiddenMembers
    {
      EGLDisplay display;
      EGLSurface surface;
      EGLContext context;
    };

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      eglMakeCurrent(hiddenMembers->display, hiddenMembers->surface, hiddenMembers->surface, hiddenMembers->context);
      currentContext = hiddenMembers;
    }

    void Context::swapBuffers()
    {
      eglSwapBuffers(hiddenMembers->display, hiddenMembers->surface);
    }

    void* Context::getCurrentOsSpecific()
    {
      return currentContext;
    }

    void Context::setCurrentOsSpecififc(void* ctx)
    {
      Context::ContextHiddenMembers* members = (Context::ContextHiddenMembers*)ctx;
      eglMakeCurrent(members->display, members->surface, members->surface, members->context);
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

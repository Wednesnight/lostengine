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
      HDC   glDeviceContext;
      HGLRC glContext;
    };

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glDeviceContext = wglGetCurrentDC();
      hiddenMembers->glContext = wglGetCurrentContext();
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      if (wglGetCurrentDC() != hiddenMembers->glDeviceContext ||
          wglGetCurrentContext() != hiddenMembers->glContext)
      {
        DOUT("current");
        wglMakeCurrent(hiddenMembers->glDeviceContext, hiddenMembers->glContext);
        currentContext = hiddenMembers;
      }
    }

    void Context::swapBuffers()
    {
      //wglSwapLayerBuffers(hiddenMembers->glDeviceContext, WGL_SWAP_MAIN_PLANE);
      SwapBuffers(hiddenMembers->glDeviceContext);
    }

    void Context::vsync(bool enable)
    {
      // FIXME: implement Context::vsync
    }

    void Context::multithreaded(bool enable)
    {
      // FIXME: implement Context::multithreaded
    }

    void* Context::getCurrentOsSpecific()
    {
	    return currentContext;
	}
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      Context::ContextHiddenMembers* members = (Context::ContextHiddenMembers*)ctx;
      wglMakeCurrent(members->glDeviceContext, members->glContext);
    }

  }
}

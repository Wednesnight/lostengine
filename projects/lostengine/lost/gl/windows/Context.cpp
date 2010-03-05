#include "lost/gl/Context.h"
#include "lost/gl/gl.h"

namespace lost
{
  namespace gl
  {
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
      Context::ContextHiddenMembers* members = new Context::ContextHiddenMembers;
	  members->glDeviceContext = wglGetCurrentDC();
	  members->glContext = wglGetCurrentContext();
      return members;
	}
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      Context::ContextHiddenMembers* members = (Context::ContextHiddenMembers*)ctx;
      wglMakeCurrent(members->glDeviceContext, members->glContext);
    }

  }
}

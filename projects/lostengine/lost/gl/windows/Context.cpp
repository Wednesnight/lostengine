#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#include "lost/common/Logger.h"

#include <boost/thread/tss.hpp>

namespace lost
{
  namespace gl
  {
    struct Context::ContextHiddenMembers
    {
      HDC   glDeviceContext;
      HGLRC glContext;
    };

    void context_cleanup(Context* p)
    {
    }
    // set to the current context in makeCurrent
    boost::thread_specific_ptr<Context> currentContext(context_cleanup);

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glDeviceContext = wglGetCurrentDC();
      hiddenMembers->glContext = wglGetCurrentContext();
      currentContext.reset(this);
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
        currentContext.reset(this);
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
	    return currentContext.get();
    }
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      ((Context*) ctx)->makeCurrent();
    }

  }
}

#include "lost/gl/Context.h"
#include "lost/gl/gl.h"

#include <boost/thread/tss.hpp>

namespace lost
{
  namespace gl
  {
    struct Context::ContextHiddenMembers
    {
      Display*    glDisplay;
      GLXDrawable glDrawable;
      GLXContext  glContext;
    };

    void context_cleanup(Context* p)
    {
    }
    // set to the current context in makeCurrent
    boost::thread_specific_ptr<Context> currentContext(context_cleanup);

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glDisplay  = glXGetCurrentDisplay();
      hiddenMembers->glDrawable = glXGetCurrentDrawable();
      hiddenMembers->glContext  = glXGetCurrentContext();
      currentContext.reset(this);
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      if (glXGetCurrentDisplay() != hiddenMembers->glDisplay ||
          glXGetCurrentDrawable() != hiddenMembers->glDrawable ||
          glXGetCurrentContext() != hiddenMembers->glContext)
      {
        glXMakeCurrent(hiddenMembers->glDisplay, hiddenMembers->glDrawable, hiddenMembers->glContext);
        currentContext.reset(this);
      }
    }

    void Context::clearCurrent()
    {
      glXMakeCurrent(hiddenMembers->glDisplay, None, NULL);
    }
    
    void Context::swapBuffers()
    {
      glXSwapBuffers(hiddenMembers->glDisplay, hiddenMembers->glDrawable);
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

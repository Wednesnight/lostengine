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
      Display*    glDisplay;
      GLXDrawable glDrawable;
      GLXContext  glContext;
    };

    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glDisplay  = glXGetCurrentDisplay();
      hiddenMembers->glDrawable = glXGetCurrentDrawable();
      hiddenMembers->glContext  = glXGetCurrentContext();
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
        currentContext = hiddenMembers;
      }
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
      return currentContext;
    }
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      Context::ContextHiddenMembers* members = (Context::ContextHiddenMembers*)ctx;
      glXMakeCurrent(members->glDisplay, members->glDrawable, members->glContext);
    }

  }
}

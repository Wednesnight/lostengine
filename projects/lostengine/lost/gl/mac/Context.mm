#include "lost/gl/Context.h"

#import <AppKit/NSOpenGL.h>

namespace lost
{
  namespace gl
  {
      
    struct Context::ContextHiddenMembers
    {
      NSOpenGLContext* glContext;
    };
    
    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glContext = [NSOpenGLContext currentContext];
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      if ([NSOpenGLContext currentContext] != hiddenMembers->glContext)
      {
        [hiddenMembers->glContext makeCurrentContext];
      }
    }

    void Context::swapBuffers()
    {
      [hiddenMembers->glContext flushBuffer];
    }

    void Context::vsync(bool enable)
    {
      GLint f = enable ? 1 : 0;
      [hiddenMembers->glContext setValues:&f forParameter:NSOpenGLCPSwapInterval];
    }

  }
}

#include "lost/application/gl/Context.h"

#import <AppKit/NSOpenGL.h>

namespace lost
{
  namespace application
  {
        
    struct Context::ContextHiddenMembers
    {
      NSOpenGLContext* glContext;
    };
    
    Context::Context()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->glContext = [NSOpenGLContext currentContext];
    }

    Context::~Context()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      [hiddenMembers->glContext makeCurrentContext];
    }

    void Context::swapBuffers()
    {
      [hiddenMembers->glContext flushBuffer];
    }

  }
}

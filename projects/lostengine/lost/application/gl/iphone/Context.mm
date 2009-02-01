#include "lost/application/gl/Context.h"

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

namespace lost
{
  namespace application
  {
    namespace gl
    {
        
      struct Context::ContextHiddenMembers
      {
        EAGLContext* glContext;
      };
      
      Context::Context()
      {
        hiddenMembers = new ContextHiddenMembers;
        hiddenMembers->glContext = [EAGLContext currentContext];
      }

      Context::~Context()
      {
        delete hiddenMembers;
      }

      void Context::makeCurrent()
      {
        [EAGLContext setCurrentContext: hiddenMembers->glContext];
      }

      void Context::swapBuffers()
      {
//        [hiddenMembers->glContext presentRenderbuffer: GL_RENDERBUFFER_OES];
      }

    }
  }
}

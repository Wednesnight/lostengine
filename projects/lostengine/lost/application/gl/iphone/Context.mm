#include "lost/application/gl/Context.h"

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

#import "lost/application/gl/iphone/GLContext.h"

namespace lost
{
  namespace application
  {
    namespace gl
    {
        
      struct Context::ContextHiddenMembers
      {
        GLContext* glContext;
      };
      
      Context::Context()
      {
        hiddenMembers = new ContextHiddenMembers;
        hiddenMembers->glContext = (GLContext*)[EAGLContext currentContext];
      }

      Context::~Context()
      {
        delete hiddenMembers;
      }

      void Context::makeCurrent()
      {
        // Make sure that you are drawing to the current context
        [EAGLContext setCurrentContext: hiddenMembers->glContext];
      }

      void Context::swapBuffers()
      {
        [hiddenMembers->glContext swapBuffers];
      }

    }
  }
}

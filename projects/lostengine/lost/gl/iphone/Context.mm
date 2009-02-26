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
      
      void Context::initialize()
      {
        hiddenMembers = new ContextHiddenMembers;
        hiddenMembers->glContext = (GLContext*)[EAGLContext currentContext];
      }

      void Context::finalize()
      {
        delete hiddenMembers;
      }

      void Context::makeCurrent()
      {
        if ((GLContext*)[EAGLContext currentContext] != hiddenMembers->glContext)
        {
          [EAGLContext setCurrentContext: hiddenMembers->glContext];
        }
      }

      void Context::swapBuffers()
      {
        [hiddenMembers->glContext swapBuffers];
      }

    }
  }
}

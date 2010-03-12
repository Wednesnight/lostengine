#include "lost/gl/Context.h"
#include "lost/gl/gl.h"
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>


namespace lost
{
  namespace gl
  {
      

    struct Context::ContextHiddenMembers
    {
      EAGLContext* context;
    };
    
    void Context::initialize()
    {
      hiddenMembers = new ContextHiddenMembers;
      hiddenMembers->context = [EAGLContext currentContext];
    }

    void Context::finalize()
    {
      delete hiddenMembers;
    }

    void Context::makeCurrent()
    {
      [EAGLContext setCurrentContext: hiddenMembers->context];
    }

    void Context::swapBuffers()
    {
      [hiddenMembers->context presentRenderbuffer:GL_RENDERBUFFER];  
    }

    void* Context::getCurrentOsSpecific()
    {
      return [EAGLContext currentContext];
    }
    
    void Context::setCurrentOsSpecififc(void* ctx)
    {
      [EAGLContext setCurrentContext:((EAGLContext*)ctx)];
    }


    void Context::vsync(bool enable)
    {
      // FIXME: implement Context::vsync
    }
    
    void Context::multithreaded(bool enable)
    {
      // FIXME: implement Context::multithreaded
    }
    
  }
}

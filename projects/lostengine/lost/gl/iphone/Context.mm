#include "lost/gl/Context.h"
#include <boost/thread/tss.hpp>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

#import "lost/gl/iphone/GLContext.h"

namespace lost
{
  namespace gl
  {
      
    /**
     Helper struct that holds the thread's currently active OpenGL context.
     This helps us with optimizing the OpenGL state changes (CGLGetParameter)
     **/
    struct ThreadState
    {
      GLContext* currentContext;
      ThreadState() : currentContext((GLContext*)[EAGLContext currentContext]) {}
    };
    boost::thread_specific_ptr<ThreadState> threadState;

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
      if (threadState.get() == 0) threadState.reset(new ThreadState);
      if (threadState->currentContext != hiddenMembers->glContext)
      {
        threadState->currentContext = hiddenMembers->glContext;
        [EAGLContext setCurrentContext: hiddenMembers->glContext];
      }
    }

    void Context::swapBuffers()
    {
      [hiddenMembers->glContext swapBuffers];
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

#ifndef LOST_APPLICATION_GL_CONTEXT_H
#define LOST_APPLICATION_GL_CONTEXT_H

#include "lost/gl/gl.h"

namespace lost
{
  namespace application
  {
    
    struct Context
    {
    private:
      struct ContextHiddenMembers;
      ContextHiddenMembers* hiddenMembers;
    public:
      Context();
      ~Context();

      void makeCurrent();
      void swapBuffers();
    };

  }
}

#endif

#ifndef LOST_APPLICATION_CONTEXT_H
#define LOST_APPLICATION_CONTEXT_H

namespace lost
{
  namespace application
  {
    
    struct GLContext
    {
      GLContext() {};
      virtual ~GLContext() {};
      
      void swapBuffers();
      
    };
    
  }
}
#endif
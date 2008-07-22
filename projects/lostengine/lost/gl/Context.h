#ifndef LOST_GL_CONTEXT_H
#define LOST_GL_CONTEXT_H

namespace lost
{
namespace gl
{

struct Context
{
  Context();
  virtual ~Context();
  
  void swapBuffers();
  
};
      
}
}
#endif
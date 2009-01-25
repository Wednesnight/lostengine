#ifndef GL_CONTEXT_H
#define GL_CONTEXT_H

#include "lost/gl/gl.h"

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

#endif

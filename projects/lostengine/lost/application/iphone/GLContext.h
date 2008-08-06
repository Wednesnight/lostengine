#ifndef LOST_APPLICATION_IPHONE_GLCONTEXT_H
#define LOST_APPLICATION_IPHONE_GLCONTEXT_H

#include "lost/gl/gl.h"
#include "lost/common/DisplayAttributes.h"

struct GLContext
{
  GLContext();
  virtual ~GLContext();
  
  void init(const lost::common::DisplayAttributes inAtt);
  void swapBuffers();
};

#endif
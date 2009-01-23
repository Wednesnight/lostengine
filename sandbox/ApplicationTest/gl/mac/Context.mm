#include "Context.h"

#import <AppKit/NSOpenGL.h>

struct Context::ContextHiddenMembers
{
  NSOpenGLContext* glContext;
};

Context::Context()
{
  hiddenMembers = new ContextHiddenMembers;
  hiddenMembers->glContext = [NSOpenGLContext currentContext];
}

Context::~Context()
{
  delete hiddenMembers;
}

void Context::makeCurrent()
{
  [hiddenMembers->glContext makeCurrentContext];
}

void Context::swapBuffers()
{
  [hiddenMembers->glContext flushBuffer];
}

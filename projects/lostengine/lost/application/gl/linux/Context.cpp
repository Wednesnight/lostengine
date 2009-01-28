#include "Context.h"

// FIXME: include GLX headers

struct Context::ContextHiddenMembers
{
  PDisplay    glDisplay;
  GLXDrawable glDrawable;
  GLXContext  glContext;
};

Context::Context()
{
  hiddenMembers = new ContextHiddenMembers;
  hiddenMembers->glDisplay  = glXGetCurrentDisplay();
  hiddenMembers->glDrawable = glXGetCurrentDrawable();
  hiddenMembers->glContext  = glXGetCurrentContext();
}

Context::~Context()
{
  delete hiddenMembers;
}

void Context::makeCurrent()
{
  glXMakeCurrent(hiddenMembers->glDisplay, hiddenMembers->glDrawable, hiddenMembers->glContext);
}

void Context::swapBuffers()
{
  glXSwapBuffers(hiddenMembers->glDisplay, hiddenMembers->glDrawable);
}

#include "Context.h"

// FIXME: include GLX headers

struct Context::ContextHiddenMembers
{
  PDisplay    glDisplay;
  GLXDrawable glDrawable;
  GLXContext  glContext;
};

void Context::initialize()
{
  hiddenMembers = new ContextHiddenMembers;
  hiddenMembers->glDisplay  = glXGetCurrentDisplay();
  hiddenMembers->glDrawable = glXGetCurrentDrawable();
  hiddenMembers->glContext  = glXGetCurrentContext();
}

void Context::finalize()
{
  delete hiddenMembers;
}

void Context::makeCurrent()
{
  if (glXGetCurrentDisplay() != hiddenMembers->glDisplay ||
      glXGetCurrentDrawable() != hiddenMembers->glDrawable ||
      glXGetCurrentContext() != hiddenMembers->glContext)
  {
    glXMakeCurrent(hiddenMembers->glDisplay, hiddenMembers->glDrawable, hiddenMembers->glContext);
  }
}

void Context::swapBuffers()
{
  glXSwapBuffers(hiddenMembers->glDisplay, hiddenMembers->glDrawable);
}

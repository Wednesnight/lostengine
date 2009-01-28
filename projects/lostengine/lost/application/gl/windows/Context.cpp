#include "../Context.h"

struct Context::ContextHiddenMembers
{
  HDC   glDeviceContext;
  HGLRC glContext;
};

Context::Context()
{
  hiddenMembers = new ContextHiddenMembers;
  hiddenMembers->glDeviceContext = wglGetCurrentDC();
  hiddenMembers->glContext = wglGetCurrentContext();
}

Context::~Context()
{
  delete hiddenMembers;
}

void Context::makeCurrent()
{
  wglMakeCurrent(hiddenMembers->glDeviceContext, hiddenMembers->glContext);
}

void Context::swapBuffers()
{
//  wglSwapLayerBuffers(hiddenMembers->glDeviceContext, WGL_SWAP_MAIN_PLANE);
  SwapBuffers(hiddenMembers->glDeviceContext);
}

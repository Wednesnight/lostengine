#include "../Context.h"

struct Context::ContextHiddenMembers
{
  HDC   glDeviceContext;
  HGLRC glContext;
};

void Context::initialize()
{
  hiddenMembers = new ContextHiddenMembers;
  hiddenMembers->glDeviceContext = wglGetCurrentDC();
  hiddenMembers->glContext = wglGetCurrentContext();
}

void Context::finalize()
{
  delete hiddenMembers;
}

void Context::makeCurrent()
{
  if (wglGetCurrentDC() != hiddenMembers->glDeviceContext ||
      wglGetCurrentContext() != hiddenMembers->glContext)
  {
    wglMakeCurrent(hiddenMembers->glDeviceContext, hiddenMembers->glContext);
  }
}

void Context::swapBuffers()
{
//  wglSwapLayerBuffers(hiddenMembers->glDeviceContext, WGL_SWAP_MAIN_PLANE);
  SwapBuffers(hiddenMembers->glDeviceContext);
}

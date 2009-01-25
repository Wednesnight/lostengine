/** This is the main header for the lgl GL abstraction layer.
 * lgl attempts to provide a common interface to gl functions that are different betwee
 * OpenGL and OpenGL ES.
 * Additionally, enums and types are rovided where required.
 * lglu.h uses these functions to reimplement glu functions so they can be used on both OpenGL and
 * OpenGL ES.
 */

#ifndef LOST_LGL_LGL_H
#define LOST_LGL_LGL_H

#include "lost/gl/gl.h"

#if defined(TARGET_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
# include "lost/lgl/lgl_gles.h"
#else
# include "lost/lgl/lgl_gl.h"
#endif

#if defined WIN32
  struct lglContext
  {
    HDC   dc;
    HGLRC c;
  };
#endif

#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_IPHONE)
void* lglGetCurrentContext();
#else
static inline void* lglGetCurrentContext()
{
#if defined WIN32
  lglContext* result = new lglContext;
  result->dc = wglGetCurrentDC();
  result->c  = wglGetCurrentContext();
  return result;
#elif defined linux
  return NULL;
#elif defined __APPLE__
  return (void*)CGLGetCurrentContext();
#endif
}
#endif

#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_IPHONE)
bool lglSetCurrentContext(void* context);
#else
static inline bool lglSetCurrentContext(void* context)
{
#if defined WIN32
  lglContext* c = (lglContext*)context;
  return (bool)wglMakeCurrent(c->dc, c->c);
#elif defined linux
  return false;
#elif defined __APPLE__
  return CGLSetCurrentContext((_CGLContextObject*)context);
#endif
}
#endif

#endif

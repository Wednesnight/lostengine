#ifndef LOST_AL_AL_H
#define LOST_AL_AL_H

#include "lost/platform/Platform.h"

#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_IPHONE)
  #import <OpenGLES/ES1/gl.h>
  #import <OpenGLES/ES1/glext.h>
#else
  #include "GLee.h"
#endif

#if defined WIN32
    // include windows al headers here
#elif defined linux
    // include linux al headers here
#elif defined __APPLE__
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#endif

#include "lost/al/Debug.h"

#endif

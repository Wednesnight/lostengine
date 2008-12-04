#ifndef LOST_AL_AL_H
#define LOST_AL_AL_H

#include "lost/platform/Platform.h"

#if defined WIN32
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#elif defined linux
    // include linux al headers here
#elif defined __APPLE__
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#endif

#include "lost/al/Debug.h"

#endif

#ifndef LOST_AL_AL_H
#define LOST_AL_AL_H

#include "lost/platform/Platform.h"
#include "lost/platform/shared_ptr.h"

#if defined WIN32
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#elif defined linux
  #include <AL/al.h>
  #include <AL/alc.h>
#elif defined __APPLE__
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#endif

#include "lost/al/Debug.h"

#endif

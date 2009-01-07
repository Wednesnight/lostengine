#ifndef LOST_AL_DEBUG_H
#define LOST_AL_DEBUG_H

#include "lost/platform/Platform.h"

// debug functions and convenience macros
// place the macros after each call to AL to detect errors

namespace lost
{
  namespace al
  {
    void debug(const char* file, int line, const char* func);
    void debug_throw(const char* file, int line, const char* func);

  }
}

#define ALDEBUG lost::al::debug(__FILE__, __LINE__, __PRETTY_FUNCTION__);
#define ALDEBUG_THROW lost::al::debug_throw(__FILE__, __LINE__, __PRETTY_FUNCTION__);

#endif

#ifndef LOST_PLATFORM_PLATFORM_H
#define LOST_PLATFORM_PLATFORM_H

#include "lost/platform/Type.h"
#include "lost/platform/Time.h"
#include "lost/platform/Paths.h"
#include "lost/platform/Thread.h"

namespace lost {
  namespace platform {
    string getClipboardString();
    bool setClipboardString(const string& str);
  }
}

#endif

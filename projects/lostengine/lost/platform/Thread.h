#ifndef LOST_PLATFORM_THREAD_H
#define LOST_PLATFORM_THREAD_H

namespace lost
{
  namespace platform
  {
    void setThreadName(const string& name);
    string getThreadName();
  }
}

#endif

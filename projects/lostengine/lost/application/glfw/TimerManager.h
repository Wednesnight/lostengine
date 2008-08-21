#ifndef LOST_APPLICATION_TIMERMANAGER_H
#define LOST_APPLICATION_TIMERMANAGER_H

#include <list>
#include "lost/application/Timer.h"

namespace lost
{
namespace application
{
struct TimerManager
{
  TimerManager();
  virtual ~TimerManager();

  void addTimer(Timer* timer);
  void removeTimer(Timer* timer);
  void updateTimers(double deltaSec);
  std::list<Timer*> timers;
};
}
}

#endif

#ifndef LOST_TIME_FORWARD_H
#define LOST_TIME_FORWARD_H

namespace lost
{
  namespace time
  {

    struct Clock;
    struct Timer;
    typedef lost::shared_ptr<Timer> TimerPtr;
    struct TimerScheduler;
    struct ThreadedTimerScheduler;

  }
}

#endif

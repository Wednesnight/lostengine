#ifndef LOST_TIME_TIMERSCHEDULER_H
#define LOST_TIME_TIMERSCHEDULER_H

#include "lost/time/Timer.h"

namespace lost
{
  namespace time
  {

    struct TimerScheduler
    {

    public:
      virtual bool startTimer(Timer* timer) = 0;
      virtual bool stopTimer(Timer* timer) = 0;
      virtual bool restartTimer(Timer* timer) = 0;

      virtual void processTimers() = 0;

      Timer* createTimer(double interval, const TimerCallback& callback, bool startImmediately = false)
      {
        Timer* timer = new Timer(this, interval, callback);
        if (startImmediately) {
          startTimer(timer);
        }
        return timer;
      }

    };

  }
}

#endif

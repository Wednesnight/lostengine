#ifndef LOST_TIME_TIMER_H
#define LOST_TIME_TIMER_H

#include "lost/time/forward.h"

#include <boost/function.hpp>

namespace lost
{
  namespace time
  {

    typedef boost::function<bool (const Timer*)> TimerCallback;

    struct Timer
    {

    private:
      TimerScheduler* scheduler;

      bool active;

      double interval;
      double time;

      TimerCallback callback;

    public:
      Timer(TimerScheduler* scheduler, double interval, const TimerCallback& callback);

      void start();
      void stop();
      void restart();

      void setInterval(double interval);
      double getInterval() const;

      void setTime(double time);
      double getTime() const;

      bool isActive() const;

      bool fire();
    };

  }
}

#endif

#include "lost/time/Timer.h"
#include "lost/time/TimerScheduler.h"

namespace lost
{
  namespace time
  {

    Timer::Timer(TimerScheduler* scheduler, double interval, const TimerCallback& callback)
    : scheduler(scheduler),
      interval(interval),
      callback(callback),
      active(false)
    {
    }

    void Timer::start()
    {
      active = scheduler->startTimer(this);
    }

    void Timer::stop()
    {
      active = scheduler->stopTimer(this);
    }

    void Timer::restart()
    {
      if (active) {
        active = scheduler->restartTimer(this);
      }
    }

    void Timer::setInterval(double interval)
    {
      this->interval = interval;
      restart();
    }

    double Timer::getInterval() const
    {
      return interval;
    }
    
    void Timer::setTime(double time)
    {
      this->time = time;
    }

    double Timer::getTime() const
    {
      return time;
    }

    bool Timer::isActive() const
    {
      return active;
    }

    bool Timer::fire()
    {
      return callback(this);
    }

  }
}

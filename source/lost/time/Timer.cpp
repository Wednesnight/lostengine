/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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
      active = scheduler->startTimer(shared_from_this());
    }

    void Timer::stop()
    {
      active = scheduler->stopTimer(shared_from_this());
    }

    void Timer::restart()
    {
      active = scheduler->restartTimer(shared_from_this());
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

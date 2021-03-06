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

#ifndef LOST_TIME_TIMERSCHEDULER_H
#define LOST_TIME_TIMERSCHEDULER_H

#include "lost/time/Timer.h"

#define LOST_TIME_TIMERSCHEDULER_STDDEV 0.00001

namespace lost
{
  namespace time
  {

    struct TimerScheduler
    {

    protected:
      double accuracy;

    public:
      TimerScheduler(double accuracy = LOST_TIME_TIMERSCHEDULER_STDDEV)
      : accuracy(accuracy)
      {
      }

      virtual ~TimerScheduler() {};
      
      virtual bool startTimer(const TimerPtr& timer) = 0;
      virtual bool stopTimer(const TimerPtr& timer) = 0;
      virtual bool restartTimer(const TimerPtr& timer) = 0;

      virtual void processTimers() = 0;

      TimerPtr createTimer(double interval, const TimerCallback& callback, bool startImmediately = false)
      {
        TimerPtr timer(new Timer(this, interval, callback));
        if (startImmediately) {
          startTimer(timer);
        }
        return timer;
      }

    };

  }
}

#endif

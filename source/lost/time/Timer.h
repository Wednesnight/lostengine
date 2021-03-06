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

#ifndef LOST_TIME_TIMER_H
#define LOST_TIME_TIMER_H

#include "lost/time/forward.h"
#include "lost/common/Function.h"

namespace lost
{
  namespace time
  {

    typedef common::Function<bool(*)(const Timer*)> TimerCallback;

    struct Timer : public enable_shared_from_this<Timer>
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

      virtual bool fire();
    };

  }
}

#endif

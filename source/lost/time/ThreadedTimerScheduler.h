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

#ifndef LOST_TIME_THREADEDTIMERSCHEDULER_H
#define LOST_TIME_THREADEDTIMERSCHEDULER_H

#include "lost/time/TimerScheduler.h"
#include "lost/time/Timer.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "lost/common/Hash.h"
#include <tinythread.h>

namespace lost
{
  namespace time
  {

    struct ThreadedTimerSchedulerEvent;
    typedef lost::shared_ptr<ThreadedTimerSchedulerEvent> ThreadedTimerSchedulerEventPtr;

    struct ThreadedTimerSchedulerEvent : public event::Event
    {
      static const event::Type& PROCESS_TIMERS() { static const event::Type d = common::djb2Hash("processTimers"); return d; }

      ThreadedTimerSchedulerEvent() : Event(PROCESS_TIMERS()) {}
      virtual ~ThreadedTimerSchedulerEvent() {}

      static ThreadedTimerSchedulerEventPtr create() { return ThreadedTimerSchedulerEventPtr(new ThreadedTimerSchedulerEvent()); }
    };

    struct ThreadedTimerScheduler : public TimerScheduler
    {

    private:
      bool active;

      event::EventDispatcherPtr eventDispatcher;

      lost::shared_ptr<tthread::thread> schedulerThread;
      tthread::mutex schedulerWaitMutex;
      tthread::condition_variable schedulerWaitCondition;

      static void __dispatch_thread_method(void* p);
      void schedulerThreadMethod();

      double nextUpdateTime;
      void updateScheduling(double requiredUpdateTime);

      list<TimerPtr> timers;
      tthread::mutex timersMutex;

      void addTimerToList(const TimerPtr& timer);
      void removeTimerFromList(const TimerPtr& timer);
      string _taskletName;
    public:
      ThreadedTimerScheduler(const string& taskletName, const event::EventDispatcherPtr& eventDispatcher = event::EventDispatcherPtr(), double accuracy = LOST_TIME_TIMERSCHEDULER_STDDEV);
      ~ThreadedTimerScheduler();

      bool startTimer(const TimerPtr& timer);
      bool stopTimer(const TimerPtr& timer);
      bool restartTimer(const TimerPtr& timer);

      void processTimers() { processTimers(ThreadedTimerSchedulerEventPtr()); }
      void processTimers(const ThreadedTimerSchedulerEventPtr& event);

    };

  }
}

#endif

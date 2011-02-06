#ifndef LOST_TIME_THREADEDTIMERSCHEDULER_H
#define LOST_TIME_THREADEDTIMERSCHEDULER_H

#include "lost/time/TimerScheduler.h"
#include "lost/time/Timer.h"
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

#include <list>

namespace lost
{
  namespace time
  {

    struct ThreadedTimerSchedulerEvent;
    typedef lost::shared_ptr<ThreadedTimerSchedulerEvent> ThreadedTimerSchedulerEventPtr;

    struct ThreadedTimerSchedulerEvent : public event::Event
    {
      static const event::Type& PROCESS_TIMERS() { static const event::Type d = "processTimers"; return d; }

      ThreadedTimerSchedulerEvent() : Event(PROCESS_TIMERS()) {}
      virtual ~ThreadedTimerSchedulerEvent() {}

      static ThreadedTimerSchedulerEventPtr create() { return ThreadedTimerSchedulerEventPtr(new ThreadedTimerSchedulerEvent()); }
    };

    struct ThreadedTimerScheduler : public TimerScheduler
    {

    private:
      bool active;

      event::EventDispatcherPtr eventDispatcher;

      lost::shared_ptr<boost::thread> schedulerThread;
      boost::mutex schedulerWaitMutex;
      boost::condition schedulerWaitCondition;
      void schedulerThreadMethod();

      double startTime;
      boost::system_time startSystemTime;
      double nextUpdateTime;
      void updateScheduling(double requiredUpdateTime);

      std::list<Timer*> timers;
      void addTimerToList(Timer* timer);
      void removeTimerFromList(Timer* timer);
      
    public:
      ThreadedTimerScheduler(const event::EventDispatcherPtr& eventDispatcher = event::EventDispatcherPtr());
      ~ThreadedTimerScheduler();

      bool startTimer(Timer* timer);
      bool stopTimer(Timer* timer);
      bool restartTimer(Timer* timer);

      void processTimers() { processTimers(ThreadedTimerSchedulerEventPtr()); }
      void processTimers(const ThreadedTimerSchedulerEventPtr& event);

    };

  }
}

#endif

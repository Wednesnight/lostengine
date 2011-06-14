#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/platform/Time.h"
#include "lost/event/Event.h"
#include "lost/event/Listener.h"

namespace lost
{
  namespace time
  {

    ThreadedTimerScheduler::ThreadedTimerScheduler(const event::EventDispatcherPtr& eventDispatcher)
    : active(true),
      eventDispatcher(eventDispatcher),
      startTime(platform::currentTimeSeconds()),
      startSystemTime(boost::get_system_time()),
      nextUpdateTime(0)
    {
      if (eventDispatcher) {
        eventDispatcher->addEventListener(ThreadedTimerSchedulerEvent::PROCESS_TIMERS(), event::makeListener(this, &ThreadedTimerScheduler::processTimers));
      }
      schedulerThread.reset(new boost::thread(boost::bind(&ThreadedTimerScheduler::schedulerThreadMethod, this)));
    }

    ThreadedTimerScheduler::~ThreadedTimerScheduler()
    {
      active = false;
      schedulerWaitCondition.notify_all();
      if (schedulerThread->joinable()) {
        schedulerThread->join();
      }
    }

    void ThreadedTimerScheduler::schedulerThreadMethod()
    {
      boost::mutex::scoped_lock waitLock(schedulerWaitMutex);
      while (active) {
        if (nextUpdateTime == 0) {
          schedulerWaitCondition.wait(waitLock);
        }
        else {
          if (!schedulerWaitCondition.timed_wait(waitLock, startSystemTime + boost::posix_time::milliseconds((nextUpdateTime - startTime) * 1000))) {
            // timeout reached, process timers
            if (eventDispatcher) {
              eventDispatcher->queueEvent(ThreadedTimerSchedulerEvent::create());
              schedulerWaitCondition.wait(waitLock);
            }
            else {
              processTimers();
            }
          }
        }
      }
    }

    void ThreadedTimerScheduler::updateScheduling(double requiredUpdateTime)
    {
        if (nextUpdateTime == 0 || requiredUpdateTime < nextUpdateTime) {
          nextUpdateTime = requiredUpdateTime;
          schedulerWaitCondition.notify_all();
        }
    }
    
    void ThreadedTimerScheduler::addTimerToList(Timer* timer)
    {
      std::list<Timer*>::iterator iter;
      for (iter = timers.begin(); iter != timers.end(); ++iter) {
        if ((*iter)->getTime() >= timer->getTime()) {
          timers.insert(iter, timer);
          break;
        }
      }

      if (iter == timers.end()) {
        timers.push_back(timer);
      }

      updateScheduling(timer->getTime());
    }

    void ThreadedTimerScheduler::removeTimerFromList(Timer* timer)
    {
      std::list<Timer*>::iterator iter;
      for (iter = timers.begin(); iter != timers.end(); ++iter) {
        if (*iter == timer) {
          break;
        }
      }
      if (iter != timers.end()) {
        timers.erase(iter);
      }

      updateScheduling(timers.size() > 0 ? timers.front()->getTime() : 0);
    }

    bool ThreadedTimerScheduler::startTimer(Timer* timer)
    {
      timer->setTime(platform::currentTimeSeconds() + timer->getInterval());
      addTimerToList(timer);
      return true;
    }

    bool ThreadedTimerScheduler::stopTimer(Timer* timer)
    {
      removeTimerFromList(timer);
      return false;
    }

    bool ThreadedTimerScheduler::restartTimer(Timer* timer)
    {
      return !stopTimer(timer) ? startTimer(timer) : true;
    }

    void ThreadedTimerScheduler::processTimers(const ThreadedTimerSchedulerEventPtr& event)
    {
      std::list<Timer*> rescheduleTimers;

      while (timers.size() > 0 && timers.front()->getTime() == nextUpdateTime) {
        Timer* timer = timers.front();
        if (timer->fire()) {
          rescheduleTimers.push_back(timer);
        }
        timers.pop_front();
      }
      nextUpdateTime = 0;
      if (eventDispatcher) {
        schedulerWaitCondition.notify_all();
      }

      for (std::list<Timer*>::iterator iter = rescheduleTimers.begin(); iter != rescheduleTimers.end(); ++iter) {
        startTimer(*iter);
      }

      if (timers.size() > 0) {
        updateScheduling(timers.front()->getTime());
      }
    }

  }
}

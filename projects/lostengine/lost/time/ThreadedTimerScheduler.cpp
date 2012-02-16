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

#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/platform/Time.h"
#include "lost/event/Event.h"
#include "lost/event/Listener.h"

namespace lost
{
  namespace time
  {

    void __dispatch_thread_method(void* func) {
      common::function<void>* target = static_cast<common::function<void>*>(func);
      if (target != NULL) {
        target->operator()();
      }
    }

    ThreadedTimerScheduler::ThreadedTimerScheduler(const string& taskletName, const event::EventDispatcherPtr& eventDispatcher)
    : active(true),
      eventDispatcher(eventDispatcher),
      startTime(platform::currentTimeSeconds()),
      startSystemTime(platform::currentTimeMilliSeconds()),
      nextUpdateTime(0),
      _taskletName(taskletName),
      threadMethod(common::bind(this, &ThreadedTimerScheduler::schedulerThreadMethod))
    {
      if (eventDispatcher) {
        eventDispatcher->addEventListener(ThreadedTimerSchedulerEvent::PROCESS_TIMERS(), event::makeListener(this, &ThreadedTimerScheduler::processTimers));
      }
      schedulerThread.reset(new tthread::thread(__dispatch_thread_method, &threadMethod));
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
      platform::setThreadName("'"+_taskletName+"' (timer scheduler)");
//      tthread::mutex::scoped_lock waitLock(schedulerWaitMutex);
      while (active) {
        if (nextUpdateTime == 0) {
          schedulerWaitCondition.wait(schedulerWaitMutex);
        }
        else {
          if (!schedulerWaitCondition.timed_wait(waitLock, startSystemTime + boost::posix_time::milliseconds((nextUpdateTime - startTime) * 1000))) {
            // timeout reached, process timers
            if (eventDispatcher) {
              eventDispatcher->queueEvent(ThreadedTimerSchedulerEvent::create());
              schedulerWaitCondition.wait(schedulerWaitMutex);
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
      list<Timer*>::iterator iter;
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
      list<Timer*>::iterator iter;
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
      list<Timer*> rescheduleTimers;

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

      for (list<Timer*>::iterator iter = rescheduleTimers.begin(); iter != rescheduleTimers.end(); ++iter) {
        startTimer(*iter);
      }

      if (timers.size() > 0) {
        updateScheduling(timers.front()->getTime());
      }
    }

  }
}

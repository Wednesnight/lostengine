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

#include "lost/lua/bindings/LostTime.h"
#include "lost/lua/lua.h"

#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/time/TimerScheduler.h"
#include "lost/time/Timer.h"

#include <boost/bind.hpp>

using namespace lost::time;

namespace lost
{
  namespace lua
  {

    bool TimerCallbackProxy(const slub::reference& targetMethod,
                            const slub::reference& targetObject, const Timer* timer)
    {
      if (targetMethod.type() == LUA_TFUNCTION) {
        if (targetObject.type() != LUA_TNIL) {
          return slub::call<bool, const slub::reference&, const Timer*>(targetMethod, targetObject, timer);
        }
        else {
          return slub::call<bool, const Timer*>(targetMethod, timer);
        }
      }
      return false;
    }
    
    TimerPtr LostTimeTimerScheduler_createTimer(TimerScheduler* scheduler, double interval,
                                                const slub::reference& targetMethod,
                                                const slub::reference& targetObject)
    {
      return scheduler->createTimer(interval, boost::bind(&TimerCallbackProxy, targetMethod, targetObject, _1));
    }
    
    void LostTimeTimerScheduler(lua_State* state)
    {
      slub::package(state, "lost").package("time")
        .clazz<TimerScheduler>("TimerScheduler")
          .method("startTimer", &TimerScheduler::startTimer)
          .method("stopTimer", &TimerScheduler::stopTimer)
          .method("restartTimer", &TimerScheduler::restartTimer)
          .method("processTimers", &TimerScheduler::processTimers)
          .method("createTimer", &LostTimeTimerScheduler_createTimer);
    }
    
    void LostTimeThreadedTimerScheduler(lua_State* state)
    {
      slub::package(state, "lost").package("time")
        .clazz<ThreadedTimerScheduler>("ThreadedTimerScheduler")
          .extends<TimerScheduler>()
          .constructor<const string&>();
    }

    void LostTimeTimer(lua_State* state)
    {
      slub::package(state, "lost").package("time")
        .clazz<Timer>("Timer")
          .constructor<TimerScheduler*, double, const TimerCallback&>()
          .method("start", &Timer::start)
          .method("stop", &Timer::stop)
          .method("restart", &Timer::restart)
          .method("setInterval", &Timer::setInterval)
          .method("getInterval", &Timer::getInterval)
          .method("setTime", &Timer::setTime)
          .method("getTime", &Timer::getTime)
          .method("isActive", &Timer::isActive)
          .method("fire", &Timer::fire);
    }
    
    void LostTime(lua_State* state)
    {
      LostTimeTimerScheduler(state);
      LostTimeThreadedTimerScheduler(state);
      LostTimeTimer(state);
    }

  }
}

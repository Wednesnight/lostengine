#include "lost/lua/bindings/LostTime.h"
#include "lost/lua/lua.h"

#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/time/TimerScheduler.h"
#include "lost/time/Timer.h"

#include <boost/bind.hpp>

using namespace lost::time;
using namespace slub;

namespace lost
{
  namespace lua
  {

    bool TimerCallbackProxy(const luabind::object& targetMethod, const luabind::object& targetObject, const Timer* timer)
    {
      if (luabind::type(targetMethod) == LUA_TFUNCTION) {
        if (luabind::type(targetObject) != LUA_TNIL) {
          return luabind::call_function<bool>(targetMethod, targetObject, timer);
        }
        else {
          return luabind::call_function<bool>(targetMethod, timer);
        }
      }
      return false;
    }
    
    TimerPtr LostTimeTimerScheduler_createTimer(TimerScheduler* scheduler, double interval, const luabind::object& targetMethod, const luabind::object& targetObject)
    {
      return scheduler->createTimer(interval, boost::bind(&TimerCallbackProxy, targetMethod, targetObject, _1));
    }
    
    void LostTimeTimerScheduler(lua_State* state)
    {
      package(state, "lost").package("time")
        .clazz<TimerScheduler>("TimerScheduler")
          .method("startTimer", &TimerScheduler::startTimer)
          .method("stopTimer", &TimerScheduler::stopTimer)
          .method("restartTimer", &TimerScheduler::restartTimer)
          .method("processTimers", &TimerScheduler::processTimers)
          .method("createTimer", &LostTimeTimerScheduler_createTimer);
    }
    
    void LostTimeThreadedTimerScheduler(lua_State* state)
    {
      package(state, "lost").package("time")
        .clazz<ThreadedTimerScheduler>("ThreadedTimerScheduler")
          .extends<TimerScheduler>()
          .constructor<const string&>();
    }

    void LostTimeTimer(lua_State* state)
    {
      package(state, "lost").package("time")
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

#include "lost/lua/bindings/LostTime.h"
#include "lost/lua/lua.h"

#include "lost/time/ThreadedTimerScheduler.h"
#include "lost/time/TimerScheduler.h"
#include "lost/time/Timer.h"

#include <boost/bind.hpp>

using namespace luabind;
using namespace lost::time;

namespace lost
{
  namespace lua
  {

    bool TimerCallbackProxy(object targetMethod, object targetObject, const Timer* timer)
    {
      if (type(targetMethod) == LUA_TFUNCTION) {
        if (type(targetObject) != LUA_TNIL) {
          return call_function<bool>(targetMethod, targetObject, timer);
        }
        else {
          return call_function<bool>(targetMethod, timer);
        }
      }
      return false;
    }
    
    Timer* LostTimeTimerScheduler_createTimer(TimerScheduler* scheduler, double interval, object targetMethod, object targetObject)
    {
      return scheduler->createTimer(interval, boost::bind(&TimerCallbackProxy, targetMethod, targetObject, _1));
    }
    
    void LostTimeTimerScheduler(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("time")
        [
          class_<TimerScheduler>("TimerScheduler")
            .def("startTimer", &TimerScheduler::startTimer)
            .def("stopTimer", &TimerScheduler::stopTimer)
            .def("restartTimer", &TimerScheduler::restartTimer)
            .def("processTimers", &TimerScheduler::processTimers)
            .def("createTimer", &LostTimeTimerScheduler_createTimer)
        ]
      ];
    }
    
    void LostTimeThreadedTimerScheduler(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("time")
        [
          class_<ThreadedTimerScheduler, TimerScheduler>("ThreadedTimerScheduler")
            .def(constructor<>())
        ]
      ];
    }

    void LostTimeTimer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("time")
        [
          class_<Timer>("Timer")
            .def(constructor<TimerScheduler*, double, const TimerCallback&>())
            .def("start", &Timer::start)
            .def("stop", &Timer::stop)
            .def("restart", &Timer::restart)
            .def("setInterval", &Timer::setInterval)
            .def("getInterval", &Timer::getInterval)
            .def("setTime", &Timer::setTime)
            .def("getTime", &Timer::getTime)
            .def("isActive", &Timer::isActive)
            .def("fire", &Timer::fire)
        ]
      ];
    }
    
    void LostTime(lua_State* state)
    {
      LostTimeTimerScheduler(state);
      LostTimeThreadedTimerScheduler(state);
      LostTimeTimer(state);
    }

  }
}

#ifndef LOST_ENGINE_GLFW_TIMERSUBSYSTEM_H
#define LOST_ENGINE_GLFW_TIMERSUBSYSTEM_H

#include <boost/shared_ptr.hpp>
#include <boost/signals.hpp>
#include <map>
#include <stdint.h>
#include <string>
#include <stdexcept>

namespace lost
{
  namespace engine
  {
    namespace glfw
    {

      /** Helper class for timer management.
       */
      struct Timer
      {
        double intervalSec;
        double passedSec;
        bool running;

        typedef boost::signal<void(double)> TimerSignalType;
        TimerSignalType fire;

        Timer(double inIntervalSec, bool inDynamic = false)
        : intervalSec(inIntervalSec), passedSec(0), running(true)
        {}

        void disable() { running = false; }
        void enable() { running = true; }

        void update(double inPassed)
        {
          if(running)
          {
            passedSec += inPassed;
            if(passedSec >= intervalSec)
            {
              fire(passedSec);
              passedSec = 0;
            }
          }
        }
      };

      /** Main timer container and manager.
       */
      template<typename Engine>
      struct TimerSubSystem
      {
        Engine& engine;
        double  lastTime;
        double timerResolution;

        typedef std::map<std::string, boost::shared_ptr<Timer> > TimerMap;
        TimerMap timers;

        TimerSubSystem(Engine& inEngine) : engine(inEngine), lastTime(0)
        {
          timerResolution = 20.0 / 1000.0; // 20 msec
        }

        void setTimerResolution(double inVal) { timerResolution = inVal;  }
        double getTimerResolution() { return timerResolution; }

        void init()
        {
          lastTime = glfwGetTime();
        }

        void add(const std::string& name, double intervalSec)
        {
          timers[name] = boost::shared_ptr<Timer>(new Timer(intervalSec));
        }

        boost::shared_ptr<Timer> timer(const std::string& name)
        {
          TimerMap::iterator pos = timers.find(name);
          if(pos != timers.end())
          {
            return pos->second;
          }
          else
          {
            throw std::runtime_error("couldn't find timer: "+name);
          }
        }

        // use this to access a timers signal object to establish connections
        Timer::TimerSignalType& operator[](const std::string& name)
        {
          return timer(name)->fire;
        }

        void enable(const std::string& name)
        {
          timer(name)->enable();
        }

        void disable(const std::string& name)
        {
          timer(name)->disable();
        }

        void update()
        {
          double now = glfwGetTime();
          double delta = now - lastTime;
          lastTime = now;
          for(TimerMap::iterator i=timers.begin(); i!=timers.end(); ++i)
          {
            i->second->update(delta);
          }
        }
      };

    }
  }
}

#endif

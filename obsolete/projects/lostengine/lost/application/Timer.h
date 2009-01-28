#ifndef LOST_APPLICATION_TIMER_H
#define LOST_APPLICATION_TIMER_H

#include <string>
#include "lost/forward/boost"
#include "lost/event/EventDispatcher.h"
#include "lost/application/TimerEvent.h"

namespace lost
{
  namespace application
  {

    struct TimerContext;
    
    struct Timer : public event::EventDispatcher
    {
      Timer(const std::string& name, double inIntervalSec);
      virtual ~Timer();

      // FIXME we need start/stop functions
      
      // FIXME: this is glfw specific, we might want to move this out of the way, kept stuttering in iphone simulator
      void update(double deltaSec);
      
      const std::string& name() { return mName; }
      boost::shared_ptr<TimerEvent> event;  

    private:
      double intervalSec;
      double passedSec;
      bool running;
      std::string mName;
      boost::shared_ptr<TimerContext> context;
    };

  }
}

#endif

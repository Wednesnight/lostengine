#ifndef LOST_APPLICATION_TIMEREVENT_H
#define LOST_APPLICATION_TIMEREVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace application
{

  struct Timer;

  struct TimerEvent : public event::Event
  {
    static const event::Type& TIMER_FIRED()
    {
      static const event::Type d = "timerFired";
      return d;
    }
  
    Timer* timer;
    double passedSec;

    TimerEvent(const event::Type inType) : Event(inType) { }
    virtual ~TimerEvent() {}
  };

}
}


#endif

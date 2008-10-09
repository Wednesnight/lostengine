#include <boost/shared_ptr.hpp>
#include "lost/application/Timer.h"

namespace lost
{
  namespace application
  {
    
    void Timer::update(double deltaSec)
    {
      passedSec += deltaSec;
      if(passedSec >= intervalSec)
      {
        boost::shared_ptr<TimerEvent> ev(new TimerEvent(TimerEvent::TIMER_FIRED()));
        ev->timer = this;
        ev->passedSec = passedSec;
        dispatchEvent(ev);
        passedSec = 0;
      }
    }

  }
}

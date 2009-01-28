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
        event->timer = this;
        event->passedSec = passedSec;
        dispatchEvent(event);
        passedSec = 0;
      }
    }

  }
}

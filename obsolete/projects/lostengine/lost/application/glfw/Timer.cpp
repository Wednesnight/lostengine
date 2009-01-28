#include <boost/shared_ptr.hpp>
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/glfw/TimerManager.h"
#include "lost/common/Logger.h"
using namespace boost;

namespace lost
{
namespace application
{
extern TimerManager* timerManagerInstance;

  Timer::Timer(const std::string& name, double inIntervalSec)
  : intervalSec(inIntervalSec), passedSec(0), running(true), mName(name)
  {
    timerManagerInstance->addTimer(this);
    event.reset(new TimerEvent(TimerEvent::TIMER_FIRED()));
  }

  Timer::~Timer()
  {
		if(timerManagerInstance)
			timerManagerInstance->removeTimer(this);
		else
			WOUT("can't delete timer, manager already gone");
  }
}
}

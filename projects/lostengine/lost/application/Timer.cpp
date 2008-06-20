#include "lost/application/Timer.h"
#include "lost/application/Application.h"

namespace lost
{
namespace application
{
extern Application* appInstance;

  Timer::Timer(const std::string& name, double inIntervalSec)
  : intervalSec(inIntervalSec), passedSec(0), running(true), mName(name)
  {
    appInstance->addTimer(this);
  }

  void Timer::update(double deltaSec)
  {
    if(running)
    {
      passedSec += deltaSec;
      if(passedSec >= intervalSec)
      {
        action(passedSec, this);
        passedSec = 0;
      }
    }
  }

  Timer::~Timer()
  {
    appInstance->removeTimer(this);
  }
}
}

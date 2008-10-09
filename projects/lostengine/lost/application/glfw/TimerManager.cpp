#include <boost/shared_ptr.hpp>
#include "lost/application/glfw/TimerManager.h"
#include "lost/common/Logger.h"

using namespace std;

namespace lost
{
namespace application
{
  TimerManager* timerManagerInstance = NULL;

  TimerManager::TimerManager()
  {
    if(timerManagerInstance != NULL)
    {
      throw std::runtime_error("timerManagerInstance wasn't NULL, did you instance more than one timerManager/application?");
    }
    timerManagerInstance = this;
  }

  TimerManager::~TimerManager() {}

  void TimerManager::updateTimers(double deltaSec)
  {
    for(list<Timer*>::iterator i = timers.begin(); i!=timers.end(); ++i)
    {
      (*i)->update(deltaSec);
    }
  }

  void TimerManager::addTimer(Timer* timer)
  {
    timers.push_back(timer);
  }

  void TimerManager::removeTimer(Timer* timer)
  {
    list<Timer*>::iterator pos = find(timers.begin(), timers.end(), timer);
    if(pos != timers.end())
    {
      timers.erase(pos);
    }
    else
    {
      WOUT("tried to remove timer "<<timer->name() << " which wasn't previously added");
    }
  };

}
}

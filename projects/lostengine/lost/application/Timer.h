#ifndef LOST_APPLICATION_TIMER_H
#define LOST_APPLICATION_TIMER_H

#include <string>
#include "lost/event/EventDispatcher.h"

namespace lost
{
namespace application
{

  struct TimerImpl;
  
struct Timer : public event::EventDispatcher
{
  Timer(const std::string& name, double inIntervalSec);
  virtual ~Timer();

  void update(double deltaSec);
  void disable() { running = false; }
  void enable() { running = true; }

  const std::string& name() { return mName; }

private:
  double intervalSec;
  double passedSec;
  bool running;
  std::string mName;
  
  boost::shared_ptr<TimerImpl> impl;
};
}
}

#endif

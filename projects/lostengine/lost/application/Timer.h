#ifndef LOST_APPLICATION_TIMER_H
#define LOST_APPLICATION_TIMER_H

#include <string>
#include <boost/signal.hpp>

namespace lost
{
namespace application
{
struct Timer
{
  Timer(const std::string& name, double inIntervalSec);
  virtual ~Timer();

  void update(double deltaSec);
  void disable() { running = false; }
  void enable() { running = true; }

  const std::string& name() { return mName; }

  boost::signal<void(double, Timer*)> action;

private:
  double intervalSec;
  double passedSec;
  bool running;
  std::string mName;
};
}
}

#endif

#ifndef LOST_APPLICATION_APPLICATION
#define LOST_APPLICATION_APPLICATION

#include "lost/common/DisplayAttributes.h"
#include "lost/common/Config.h"
#include <boost/signal.hpp>
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"
#include <list>
#include "lost/application/GlfwAdapter.h"
#include "lost/application/ResizeEvent.h"

namespace lost
{
namespace application
{

struct Timer;

struct Application : public event::EventDispatcher
{
public:
  Application();
  virtual ~Application();

  void run();
  void quit();

  common::DisplayAttributes displayAttributes;
private:
  GlfwAdapter adapter;

  friend struct lost::application::Timer;
  void addTimer(Timer* timer);
  void removeTimer(Timer* timer);

  // internal event handlers
  void handleResize(boost::shared_ptr<ResizeEvent> ev);


  // FIXME: create member instances of events we'Re sending to supress superfluous new/delete calls
  // since the current eventDispatcher instance is synchroous and not reentrant, we can assume this optimisation

  void updateTimers(double deltaSec);
  bool running;
  std::list<Timer*> timers;
};
}
}

#endif

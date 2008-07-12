#ifndef LOST_APPLICATION_APPLICATION
#define LOST_APPLICATION_APPLICATION

#include "lost/common/DisplayAttributes.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/GlfwAdapter.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/TimerManager.h"

namespace lost
{
namespace application
{

struct Application : public event::EventDispatcher
{
public:
  Application();
  virtual ~Application();

  void run();
  void quit();

  common::DisplayAttributes displayAttributes;
  
private:
  GlfwAdapter   adapter;
  TimerManager  timerManager;
  bool          running;

  void          handleResize(boost::shared_ptr<ResizeEvent> ev);
};
}
}

#endif

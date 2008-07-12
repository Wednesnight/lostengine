#ifndef LOST_APPLICATION_APPLICATION
#define LOST_APPLICATION_APPLICATION

#include "lost/common/DisplayAttributes.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/GlfwAdapter.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/TimerManager.h"
#include "lost/lua/State.h"
#include "lost/resource/Loader.h"

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

  common::DisplayAttributes             displayAttributes;
  boost::shared_ptr<resource::Loader>   loader;
  boost::shared_ptr<lua::State>         interpreter;
  
private:
  GlfwAdapter   adapter;
  TimerManager  timerManager;
  bool          running;

  void          handleResize(boost::shared_ptr<ResizeEvent> ev);
};
}
}

#endif

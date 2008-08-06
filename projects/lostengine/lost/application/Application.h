#ifndef LOST_APPLICATION_APPLICATION
#define LOST_APPLICATION_APPLICATION

#include "lost/common/DisplayAttributes.h"
#include "lost/event/EventDispatcher.h"
#include "lost/lua/State.h"
#include "lost/resource/Loader.h"
#include "lost/application/GLContext.h"
#include "lost/application/ResizeEvent.h"

struct LostApplicationImpl;  

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
  lost::application::GLContext          glc;

private:
  boost::shared_ptr<LostApplicationImpl> impl;
  void handleResize(boost::shared_ptr<lost::application::ResizeEvent> ev);      
  
};

extern Application* appInstance;

}
}

#endif

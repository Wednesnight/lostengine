#ifndef LOST_APPLICATION_APPLICATION
#define LOST_APPLICATION_APPLICATION

#include "lost/common/DisplayAttributes.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/ResizeEvent.h"
#include "lost/lua/State.h"
#include "lost/resource/Loader.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace application
{

struct ApplicationImpl;  
  
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
  lost::gl::Context                     glc;

private:
  boost::shared_ptr<ApplicationImpl> impl;
};

extern Application* appInstance;

}
}

#endif

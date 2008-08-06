#ifndef LOST_APPLICATION_GLFWADAPTER_H
#define LOST_APPLICATION_GLFWADAPTER_H

#include "lost/event/EventDispatcher.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/math/Vec2.h"
#include "lost/application/glfw/TimerManager.h"

namespace lost
{
namespace application
{

struct GlfwAdapter
{
  event::EventDispatcher* target;
  
  GlfwAdapter(event::EventDispatcher* inTarget);
  virtual ~GlfwAdapter();

  void init(const common::DisplayAttributes& displayAttributes); // call this to configure adapter before running it, creates OpenGL context 
  void run(); // runs the main loop
  void quit(); // quits the main loop
  void terminate(); // call this to shut the adapter down whe your application has quit

  // private state
  int displayHeight;   
  lost::application::TimerManager  timerManager;  
  bool          running;  
};

}
}

#endif
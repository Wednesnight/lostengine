#ifndef LOST_APPLICATION_GLFW_LOSTAPPLICATIONIMPL_H
#define LOST_APPLICATION_GLFW_LOSTAPPLICATIONIMPL_H

#include "lost/application/glfw/GlfwAdapter.h"
#include "lost/application/glfw/TimerManager.h"
#include "lost/application/ResizeEvent.h"
#include "lost/event/EventDispatcher.h"

struct LostApplicationImpl
{
  lost::application::GlfwAdapter   adapter;
  lost::application::TimerManager  timerManager;
  lost::event::EventDispatcher*  delegate;
  bool          initialised;
  bool          running;

  LostApplicationImpl(lost::event::EventDispatcher* target);  
  void handleResize(boost::shared_ptr<lost::application::ResizeEvent> ev);      
  void init();
};


#endif
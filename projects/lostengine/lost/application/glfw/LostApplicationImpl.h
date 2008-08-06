#ifndef LOST_APPLICATION_GLFW_LOSTAPPLICATIONIMPL_H
#define LOST_APPLICATION_GLFW_LOSTAPPLICATIONIMPL_H

#include "lost/application/glfw/GlfwAdapter.h"
#include "lost/event/EventDispatcher.h"

struct LostApplicationImpl
{
  lost::application::GlfwAdapter   adapter;

  LostApplicationImpl(lost::event::EventDispatcher* target);  
};


#endif
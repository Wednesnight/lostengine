#ifndef LOST_APPLICATION_GLFWADAPTER_H
#define LOST_APPLICATION_GLFWADAPTER_H

#include "lost/event/EventDispatcher.h"
#include "lost/common/DisplayAttributes.h"

struct ApplicationAdapterState;

struct GlfwAdapter
{
  lost::event::EventDispatcher* target;
  
  GlfwAdapter(lost::event::EventDispatcher* inTarget);
  virtual ~GlfwAdapter();

  void init(const lost::common::DisplayAttributes& displayAttributes); // call this to configure adapter before running it, creates OpenGL context 
  void run(); // runs the main loop
  void quit(); // quits the main loop
  void terminate(); // call this to shut the adapter down whe your application has quit
    
  boost::shared_ptr<ApplicationAdapterState> state;
};

#endif
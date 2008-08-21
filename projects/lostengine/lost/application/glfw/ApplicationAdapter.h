#ifndef LOST_APPLICATION_APPLICATIONADAPTER_H
#define LOST_APPLICATION_APPLICATIONADAPTER_H

#include "lost/event/EventDispatcher.h"
#include "lost/common/DisplayAttributes.h"

struct ApplicationAdapterState;

// this is the interface to the plattform specific part of the application.
// each platform needs to implement a suitable ApplicationAdapter.cpp and link withe the project
struct ApplicationAdapter
{
  lost::event::EventDispatcher* target;

  ApplicationAdapter(lost::event::EventDispatcher* inTarget);
  virtual ~ApplicationAdapter();

  void init(const lost::common::DisplayAttributes& displayAttributes); // call this to configure adapter before running it, creates OpenGL context
  void run(); // runs the main loop
  void swapBuffers(); // tell the current gl context to flip buffers
  void quit(); // quits the main loop
  void terminate(); // call this to shut the adapter down whe your application has quit

  boost::shared_ptr<ApplicationAdapterState> state;
};

#endif

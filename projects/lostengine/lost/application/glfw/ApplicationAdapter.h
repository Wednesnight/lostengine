#ifndef LOST_APPLICATION_APPLICATIONADAPTER_H
#define LOST_APPLICATION_APPLICATIONADAPTER_H

#include "lost/event/EventDispatcher.h"
#include "lost/common/DisplayAttributes.h"
#include <boost/thread/thread.hpp>
#include "lost/application/MainLoop.h"

struct ApplicationAdapterState;

// this is the interface to the plattform specific part of the application.
// each platform needs to implement a suitable ApplicationAdapter.cpp and link with the project
struct ApplicationAdapter
{
  lost::event::EventDispatcher* target;

  ApplicationAdapter(lost::event::EventDispatcher* inTarget);
  virtual ~ApplicationAdapter();

  void init(const boost::shared_ptr<lost::common::DisplayAttributes>& displayAttributes); // call this to configure adapter before running it, creates OpenGL context
  void run(); // runs the main loop
  void swapBuffers(); // tell the current gl context to flip buffers
  void quit(); // quits the main loop
  void terminate(); // call this to shut the adapter down whe your application has quit

  boost::mutex queueMutex;
  boost::shared_ptr<std::list<boost::shared_ptr<lost::event::Event> > > eventQueue;
  void queueEvent(const boost::shared_ptr<lost::event::Event>& event); // call this to queue the given event. will be dispatched when processEvents() is called
  void processEvents(const double& timeoutInSeconds = 0); // call this to signal queued events

  boost::shared_ptr<lost::application::MainLoop> mainLoop; 
  boost::shared_ptr<boost::thread> mainLoopThread;

  boost::shared_ptr<ApplicationAdapterState> state;
};

#endif

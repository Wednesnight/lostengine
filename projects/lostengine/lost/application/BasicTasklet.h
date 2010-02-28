#ifndef LOST_APPLICATION_BASICTASKLET_H
#define LOST_APPLICATION_BASICTASKLET_H

#include "lost/application/QueueEvent.h"
#include "lost/application/ProcessEvent.h"

namespace lost
{
namespace event 
{ 
  struct EventDispatcher; typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr;
  struct Event; typedef lost::shared_ptr<Event> EventPtr;    
}
namespace application
{

struct BasicTasklet
{
  event::EventDispatcherPtr       eventDispatcher;
  // if true, only runs the main loop once a low level event arrives
  bool waitForEvents;

  BasicTasklet();
  virtual ~BasicTasklet();
  
  void queueApplicationEvent(event::EventPtr event);
  void dispatchApplicationEvent(event::EventPtr event);
  void processApplicationEvents();  

  virtual void init() {}; // will becalled before start
  virtual bool start() { return false; }; // override this, starts tasklet, potentially on separate thread
  virtual bool stop() { return false; }; // override this, tells tasklet to stop and shutdown
  virtual bool startup() { return false;}; // called once on worker thread, return false if startup fails
  virtual bool update() {return false; }; // called repeatedly on worker thread, return false if you want to shutdown
  virtual bool shutdown() {return false;}; // called once on worker thread, return false if shutdown failed

  virtual void render() {};
  virtual void processEvents() {};

};

}
}

#endif

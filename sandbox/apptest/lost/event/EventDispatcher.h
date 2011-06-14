#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include "lost/event/forward.h"
#include "lost/event/Event.h"
#include "lost/event/Signal.h"
#include "lost/event/Connection.h"
#include "EASTL/map.h"
#include "EASTL/list.h"
#include "tinythread.h"

namespace lost
{
namespace event
{

struct EventDispatcher
{
  EventDispatcher();
  virtual ~EventDispatcher();

  Connection addEventListener(const event::Type& evType, ListenerPtr listener);
  void removeEventListener(const Connection& connection);

  void dispatchEvent(EventPtr event);

  /**
   * call this to queue the given event. will be dispatched when processEvents() is called
   */
  void queueEvent(const event::EventPtr& event);
  /**
   * call this to signal queued events
   */
  void processEvents();
  void wakeup();
  void waitForEvents();
  void waitForEvent(const lost::event::Type& type);
  void clear();
  Connection attachTo(const EventDispatcherPtr& target, const lost::event::Type& type);
  uint32_t numListeners();
  
  eastl::map<event::Type, SignalPtr> eventType2signal;
  tthread::mutex queueMutex;
  eastl::list<EventPtr> eventQueue;
  tthread::condition_variable waitEventCondition;
  tthread::mutex waitEventMutex;
};

}
}

#endif
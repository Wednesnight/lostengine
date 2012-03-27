/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include "lost/event/forward.h"
#include "lost/event/Event.h"
#include "lost/event/Signal.h"
#include "lost/event/Connection.h"
#include "tinythread.h"

namespace lost
{
namespace event
{

struct EventDispatcher
{
  EventDispatcher();
  virtual ~EventDispatcher();

  ConnectionPtr addEventListener(const event::Type& evType, ListenerPtr listener);
  void removeEventListener(const ConnectionPtr& connection);

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
  ConnectionPtr attachTo(const EventDispatcherPtr& target, const lost::event::Type& type);
  uint32_t numListeners();
  
  Pool* pool;
  map<event::Type, SignalPtr> eventType2signal;
  tthread::mutex queueMutex;
  list<EventPtr> eventQueue;
  tthread::condition_variable waitEventCondition;
  tthread::mutex waitEventMutex;
};

}
}

#endif

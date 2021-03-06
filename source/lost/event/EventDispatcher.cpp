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

#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"
#include "lost/common/Logger.h"
#include "lost/event/Pool.h"

using namespace tthread;

namespace lost
{
namespace event
{

EventDispatcher::EventDispatcher()
{
  pool = Pool::instance();
}

EventDispatcher::~EventDispatcher()
{
}

uint32_t EventDispatcher::numListeners()
{
  return eventType2signal.size();
}

ConnectionPtr EventDispatcher::addEventListener(const event::Type& evType, ListenerPtr listener)
{
  uint32_t result;
  
  SignalPtr signal = eventType2signal[evType];
  if(!signal)
  {
    signal.reset(new Signal);
    eventType2signal[evType] = signal;
  }
  result = signal->addListener(listener);
  
  return ConnectionPtr(new Connection(evType, result));
}

void EventDispatcher::removeEventListener(const ConnectionPtr& connection)
{
  SignalPtr signal = eventType2signal[connection->eventType];
  if(signal)
  {
    signal->removeListener(connection->index);
  }
}

void EventDispatcher::dispatchEvent(EventPtr event)
{
  map<event::Type, SignalPtr>::iterator pos = eventType2signal.find(event->type);
  if(pos != eventType2signal.end())
  {
    pos->second->call(event);
  }
}

void EventDispatcher::clear()
{
  eventType2signal.clear();
}

void EventDispatcher::queueEvent(const event::EventPtr& event)
{
  queueMutex.lock();
  eventQueue.push_back(event);
  queueMutex.unlock();
  waitEventCondition.notify_one();
}

void EventDispatcher::wakeup()
{
  // queue dummy event to get waitForEvents() to return
  queueEvent(EventPtr(new Event(0)));
}

void EventDispatcher::processEvents()
{
  if (eventQueue.size() > 0)
  {
    do
    {
      dispatchEvent(eventQueue.front());
      queueMutex.lock();
      eventQueue.pop_front();
      queueMutex.unlock();
    } while(eventQueue.size() > 0);
  }
}

void EventDispatcher::waitForEvents()
{
  lock_guard<mutex> lock(waitEventMutex);
  while(eventQueue.size() <= 0)
  {
    waitEventCondition.wait(waitEventMutex);
  }
}

void EventDispatcher::waitForEvent(const lost::event::Type& type)
{
  lock_guard<mutex> lock(waitEventMutex);
  bool done = false;
  list<EventPtr>::iterator pos = eventQueue.begin();
  while(!done)
  {
    waitEventCondition.wait(waitEventMutex);
    for (; pos != eventQueue.end(); ++pos) {
      done = (*pos)->type == type;
      if (done) {
        dispatchEvent(*pos);
        queueMutex.lock();
        eventQueue.erase(pos);
        queueMutex.unlock();
        break;
      }
    }
    --pos;
  }
}

ConnectionPtr EventDispatcher::attachTo(const EventDispatcherPtr& target, const lost::event::Type& type)
{
  return target->addEventListener(type, makeListener(this, &EventDispatcher::queueEvent));
}

}
}

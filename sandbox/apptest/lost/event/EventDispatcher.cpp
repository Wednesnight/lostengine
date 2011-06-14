#include "lost/event/EventDispatcher.h"
#include "lost/event/Listener.h"

using namespace tthread;

namespace lost
{
namespace event
{

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

uint32_t EventDispatcher::numListeners()
{
  return eventType2signal.size();
}

Connection EventDispatcher::addEventListener(const event::Type& evType, ListenerPtr listener)
{
  uint32_t result;
  
  SignalPtr signal = eventType2signal[evType];
  if(!signal)
  {
    signal.reset(new Signal);
    eventType2signal[evType] = signal;
  }
  result = signal->addListener(listener);
  
  return Connection(evType, result);
}

void EventDispatcher::removeEventListener(const Connection& connection)
{
  SignalPtr signal = eventType2signal[connection.eventType];
  if(signal)
  {
    signal->removeListener(connection.index);
  }
}

void EventDispatcher::dispatchEvent(EventPtr event)
{
  eastl::map<event::Type, SignalPtr>::iterator pos = eventType2signal.find(event->type);
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
  queueEvent(EventPtr(new Event("")));
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
  eastl::list<EventPtr>::iterator pos = eventQueue.begin();
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

Connection EventDispatcher::attachTo(const EventDispatcherPtr& target, const lost::event::Type& type)
{
  return target->addEventListener(type, makeListener(this, &EventDispatcher::queueEvent));
}

}
}

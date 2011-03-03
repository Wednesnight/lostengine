#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"

#include <boost/bind.hpp>

namespace lost
{
  namespace event
  {
    typedef boost::signal<void (EventPtr)> EventSignal;
    typedef lost::shared_ptr<EventSignal>  EventSignalPtr;    

    struct EventSignalPtrMap : public std::map<lost::event::Type,EventSignalPtr> {};
    
    EventDispatcher::EventDispatcher()
    {
      eventQueue.reset(new std::list<lost::shared_ptr<lost::event::Event> >());
      listeners = new EventSignalPtrMap();
    }
    
    EventDispatcher::~EventDispatcher()
    {
      delete listeners;
    }

    boost::uint32_t EventDispatcher::numListeners()
    {
      return listeners->size();
    }
    
    boost::signals::connection EventDispatcher::addEventListener(const lost::event::Type& type, EventListenerFunc callback)
    {
      EventSignalPtrMap::iterator pos = (*listeners).find(type);
      if(pos == (*listeners).end())
      {
        EventSignalPtr(new EventSignal);
        (*listeners)[type] =  EventSignalPtr(new EventSignal);
      }
      return (*listeners)[type]->connect(callback);
    }
    
    void EventDispatcher::removeEventListener(const boost::signals::connection& connection)
    {
      connection.disconnect();
    }
    
    void EventDispatcher::dispatchEvent(EventPtr event)
    {
      EventSignalPtrMap::iterator pos = (*listeners).find(event->type);
      if(pos != (*listeners).end())
      {
        (*(pos->second))(event);
      }
    }
    
    void EventDispatcher::clear()
    {
      (*listeners).clear();
    }

    void EventDispatcher::waitForEvents()
    {
      boost::unique_lock<boost::mutex> lock(waitEventMutex);
      while(eventQueue->size() <= 0)
      {
        waitEventCondition.wait(lock);
      }
    }

    void EventDispatcher::waitForEvent(const lost::event::Type& type)
    {
      boost::unique_lock<boost::mutex> lock(waitEventMutex);
      bool done = false;
      std::list<lost::shared_ptr<lost::event::Event> >::iterator pos = eventQueue->begin();
      while(!done)
      {
        waitEventCondition.wait(lock);
        for (; pos != eventQueue->end(); ++pos) {
          done = (*pos)->type == type;
          if (done) {
            dispatchEvent(*pos);
            queueMutex.lock();
            eventQueue->erase(pos);
            queueMutex.unlock();
            break;
          }
        }
        --pos;
      }
    }

    void EventDispatcher::wakeup()
    {
      // queue dummy event to get waitForEvents() to return
      queueEvent(EventPtr(new Event("")));
    }
    
    void EventDispatcher::queueEvent(const lost::shared_ptr<lost::event::Event>& event)
    {
      queueMutex.lock();
      eventQueue->push_back(event);
      queueMutex.unlock();
      waitEventCondition.notify_one();
    }
    
    void EventDispatcher::processEvents()
    {
      if (eventQueue->size() > 0)
      {
        do
        {
          dispatchEvent(eventQueue->front());
          queueMutex.lock();
          eventQueue->pop_front();
          queueMutex.unlock();
        } while(eventQueue->size() > 0);
      }
    }

    boost::signals::connection EventDispatcher::attachTo(const EventDispatcherPtr& target, const lost::event::Type& type)
    {
      return target->addEventListener(type, bind(&EventDispatcher::queueEvent, this, _1));
    }

  }
}

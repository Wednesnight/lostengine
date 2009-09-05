#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/signal.hpp>
#include "lost/event/EventDispatcher.h"
#include "lost/common/Logger.h"
#include "lost/platform/Platform.h"

namespace lost
{
  namespace event
  {
    typedef boost::signal<void (EventPtr)>             EventSignal;
    typedef lost::shared_ptr<EventSignal>             EventSignalPtr;    
//    typedef std::map<lost::event::Type,EventSignalPtr> EventSignalPtrMap;

    struct EventDispatcherHiddenMembers
    {
      boost::mutex queueMutex;
      lost::shared_ptr<std::list<lost::shared_ptr<lost::event::Event> > > eventQueue;

      boost::mutex waitEventMutex;
      boost::condition waitEventCondition;
    };
    
    struct EventSignalPtrMap : public std::map<lost::event::Type,EventSignalPtr> {};
    
    EventDispatcher::EventDispatcher()
    {
      hiddenMembers.reset(new EventDispatcherHiddenMembers);
      listeners = new EventSignalPtrMap();
    }
    
    EventDispatcher::~EventDispatcher()
    {
      delete listeners;
    }

    uint32_t EventDispatcher::numListeners()
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
      DOUT("EventDispatcher::removeEventListener()");
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

    boost::mutex waitEventMutex;
    boost::condition waitEventCondition;
    void EventDispatcher::waitForEvents()
    {
      boost::unique_lock<boost::mutex> lock(waitEventMutex);
      while(!hiddenMembers->eventQueue || !(hiddenMembers->eventQueue->size() > 0))
      {
        waitEventCondition.wait(lock);
      }
    }

    void EventDispatcher::wakeup()
    {
      // queue dummy event to get waitForEvents() to return
      queueEvent(EventPtr(new Event("")));
    }
    
    void EventDispatcher::queueEvent(const lost::shared_ptr<lost::event::Event>& event)
    {
      hiddenMembers->queueMutex.lock();
      if (!hiddenMembers->eventQueue) hiddenMembers->eventQueue.reset(new std::list<lost::shared_ptr<lost::event::Event> >());
      hiddenMembers->eventQueue->push_back(event);
      hiddenMembers->queueMutex.unlock();
      waitEventCondition.notify_one();      
    }
    
    void EventDispatcher::processEvents(const double& timeoutInMilliSeconds)
    {
      if (hiddenMembers->eventQueue && hiddenMembers->eventQueue->size() > 0)
      {
        const double startTime = lost::platform::currentTimeMilliSeconds();
        do
        {
          dispatchEvent(hiddenMembers->eventQueue->front());
          hiddenMembers->queueMutex.lock();
          hiddenMembers->eventQueue->pop_front();
          hiddenMembers->queueMutex.unlock();
        } while(hiddenMembers->eventQueue->size() > 0 && (timeoutInMilliSeconds == 0 || lost::platform::currentTimeMilliSeconds() - startTime < timeoutInMilliSeconds));
      }
    }
    
  }
}

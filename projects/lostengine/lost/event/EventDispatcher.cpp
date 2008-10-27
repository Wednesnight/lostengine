#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/event/EventDispatcher.h"

namespace lost
{
  namespace event
  {
    typedef boost::signal<void (EventPtr)>             EventSignal;
    typedef boost::shared_ptr<EventSignal>             EventSignalPtr;    
//    typedef std::map<lost::event::Type,EventSignalPtr> EventSignalPtrMap;
    struct EventSignalPtrMap : public std::map<lost::event::Type,EventSignalPtr> {};
    
    EventDispatcher::EventDispatcher()
    {
      listeners = new EventSignalPtrMap();
    }
    
    EventDispatcher::~EventDispatcher()
    {
      delete listeners;
    }
    
    void EventDispatcher::addEventListener(const lost::event::Type& type, EventListenerFunc callback)
    {
      EventSignalPtrMap::iterator pos = (*listeners).find(type);
      if(pos == (*listeners).end())
      {
        EventSignalPtr(new EventSignal);
        (*listeners)[type] =  EventSignalPtr(new EventSignal);
      }
      (*listeners)[type]->connect(callback);
    }
    
    void EventDispatcher::removeEventListener(const lost::event::Type& type, EventListenerFunc callback)
    {
      //
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
    
  }
}

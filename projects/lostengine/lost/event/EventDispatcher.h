#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/event/Event.h"

namespace lost
{
  namespace event
  {
    typedef boost::shared_ptr<lost::event::Event>               EventPtr;
    typedef boost::signal<void (EventPtr)>                      EventSignal;
    typedef boost::shared_ptr<boost::signal<void (EventPtr)> >  EventSignalPtr;    
    typedef std::map<lost::event::Type,EventSignalPtr>          EventSignalPtrMap;
    typedef boost::function<void (EventPtr)>                    EventListenerFunc;

    struct EventDispatcher
    {
      EventSignalPtrMap listeners;

      void addEventListener( const lost::event::Type& type, EventListenerFunc callback )
      {
        EventSignalPtrMap::iterator pos = listeners.find(type);
        if(pos == listeners.end())
        {
          EventSignalPtr(new EventSignal);
          listeners[type] =  EventSignalPtr(new EventSignal);
        }
        listeners[type]->connect(callback);
      }

      void dispatchEvent(EventPtr event)
      {
        EventSignalPtrMap::iterator pos = listeners.find(event->type);
        if(pos != listeners.end())
        {
          (*(pos->second))(event);
        }
      }
    };
  }
}

#endif


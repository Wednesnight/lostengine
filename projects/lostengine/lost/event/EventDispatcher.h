#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/event/Event.h"
#include <stdexcept>

namespace lost
{
  namespace event
  {
    typedef boost::shared_ptr<lost::event::Event>               EventPtr;
    typedef boost::signal<void (EventPtr)>                      EventSignal;
    typedef boost::shared_ptr<boost::signal<void (EventPtr)> >  EventSignalPtr;    
    typedef std::map<lost::event::Type,EventSignalPtr>          EventSignalPtrMap;
    typedef boost::function<void (EventPtr)>                    EventListenerFunc;


    /** since the event system only deals with Event*, all client code
     * would have to cast the incoming types. This is not acceptable most of the times since
     * the interfaces would lose information. In order to ease the handling, you can decorate
     * your handler function with receive<>() upon registration. Here's an example:
     * 
     * void myHandler(shared_ptr<MouseEvent> mev) {}
     * dispatcher.addEventListener(MouseEvent::MOUSE_DOWN, myHandler); // this would fail since cast from Event* to MouseEvent* can't be performed automatically
     * dispatcher.addEventListener(MouseEvent::MOUSE_DOWN, receive<MouseEvent>(myHandler)); // this will work since receive<>() can actually receive an Event*, but will perform the cast for you and check the result.
     */
    template<typename TO>
    struct receive
    {
      boost::function<void(boost::shared_ptr<TO>)> func;
      receive(boost::function<void(boost::shared_ptr<TO>)> f) : func(f) {}
      void operator()(boost::shared_ptr<Event> ev)
      {
        boost::shared_ptr<TO> p = boost::dynamic_pointer_cast<TO>(ev);
        if(!p) {throw std::runtime_error("couldn't cast event pointers");}
        func(p);
      }  
    };
    
    struct EventDispatcher
    {
      EventSignalPtrMap listeners;

      EventDispatcher() {}
      virtual ~EventDispatcher() {}
      
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
      
      void clear()
      {
        listeners.clear();
      }
    };
  }
}

#endif


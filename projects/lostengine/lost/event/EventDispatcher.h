#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include "lost/forward/boost/shared_ptr.hpp"
#include <boost/function.hpp>
#include "lost/event/Event.h"
#include <stdexcept>

namespace lost
{
  namespace event
  {
    typedef boost::shared_ptr<lost::event::Event> EventPtr;
    typedef boost::function<void (EventPtr)>      EventListenerFunc;

    struct EventSignalPtrMap;
    struct EventDispatcher
    {
      EventSignalPtrMap* listeners;

      EventDispatcher();
      virtual ~EventDispatcher();
      
      void addEventListener(const lost::event::Type& type, EventListenerFunc callback);
      void removeEventListener(const lost::event::Type& type, EventListenerFunc callback);
      virtual void dispatchEvent(EventPtr event);
      void clear();
    };
  }
}

#endif

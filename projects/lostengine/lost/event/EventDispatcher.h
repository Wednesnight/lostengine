#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include "lost/forward/boost"
#include <boost/function.hpp>
#include <boost/signal.hpp>
#include <boost/thread/mutex.hpp>
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
      
      boost::signals::connection addEventListener(const lost::event::Type& type, EventListenerFunc callback);
      void removeEventListener(const boost::signals::connection& connection);
      virtual void dispatchEvent(EventPtr event);
      void clear();

      boost::mutex queueMutex;
      boost::shared_ptr<std::list<boost::shared_ptr<lost::event::Event> > > eventQueue;

      void queueEvent(const boost::shared_ptr<lost::event::Event>& event); // call this to queue the given event. will be dispatched when processEvents() is called
      void processEvents(const double& timeoutInSeconds = 0); // call this to signal queued events
    };
  }
}

#endif

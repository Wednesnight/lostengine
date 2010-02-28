#ifndef LOST_EVENT_EVENTDISPATCHER_H
#define LOST_EVENT_EVENTDISPATCHER_H

#include <boost/function.hpp>
#include <boost/signal.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include "lost/event/Event.h"
#include <stdexcept>

namespace boost
{
struct mutex;
struct condition_variable_any;
typedef condition_variable_any condition;
}

namespace lost
{
  namespace event
  {
    typedef lost::shared_ptr<lost::event::Event> EventPtr;
    typedef boost::function<void (EventPtr)>      EventListenerFunc;

    struct EventDispatcher;
    typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr;

    struct EventSignalPtrMap;
    struct EventDispatcherHiddenMembers;
    struct EventDispatcher
    {
      EventSignalPtrMap* listeners;

      EventDispatcher();
      virtual ~EventDispatcher();
      
      boost::signals::connection addEventListener(const lost::event::Type& type, EventListenerFunc callback);
      void removeEventListener(const boost::signals::connection& connection);

      virtual void dispatchEvent(EventPtr event);

      void clear();

      /**
       * for debugging purposes
       */
      uint32_t numListeners();

      /**
       * returns if at least one event was queued
       */
      void waitForEvents();
      /**
       * cancells waitForEvents()
       */
      void wakeup();

      /**
       * call this to queue the given event. will be dispatched when processEvents() is called
       */
      void queueEvent(const lost::shared_ptr<lost::event::Event>& event);
      /**
       * call this to signal queued events
       */
      void processEvents();

    private:
      boost::mutex queueMutex;
      lost::shared_ptr<std::list<lost::shared_ptr<lost::event::Event> > > eventQueue;
      
      boost::mutex waitEventMutex;
      boost::condition waitEventCondition;
    };
  }
}

#endif

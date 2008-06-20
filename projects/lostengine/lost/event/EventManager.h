#ifndef LOST_EVENT_EVENTMANAGER_H
#define LOST_EVENT_EVENTMANAGER_H

#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/any.hpp>

#include "lost/event/EventTypes.h"

namespace lost
{
  namespace event
  {

    typedef boost::signal<void (boost::any&)>                                      EventManagerSignal;
    typedef std::map<lost::event::EventType, boost::weak_ptr<EventManagerSignal> > EventManagerMap;

    struct EventManager
    {
      EventManagerMap signals;

      boost::signals::connection registerEventListener( const lost::event::EventType type, boost::function<void (boost::any&)> callback )
      {
        boost::signals::connection result;
        if (signals.find( type ) != signals.end())
        {
          if (boost::shared_ptr<EventManagerSignal> signal = signals[type].lock())
          {
            result = signal->connect( callback );
          }
        }
        return result;
      }

      void registerEventServer( const lost::event::EventType type, const boost::shared_ptr<EventManagerSignal>& signal )
      {
        if (signals.find( type ) == signals.end())
        {
          signals[type] = signal;
        }
      }
      void unregisterEventServer( const lost::event::EventType type )
      {
        if (signals.find( type ) != signals.end())
        {
          signals.erase( type );
        }
      }
    };

  }
}

#endif


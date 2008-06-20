#include <boost/bind.hpp>
#include "lost/event/EventManager.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"

TEST(eventmanager)
{
  lost::event::EventManager manager;
  CHECK(manager.signals.size() == 0);
}

TEST(eventmanager_server)
{
  lost::event::EventManager                          manager;
  boost::shared_ptr<lost::event::EventManagerSignal> server;

  manager.registerEventServer( lost::event::UndefinedEventType, server );

  CHECK(manager.signals.size() == 1);
}

struct EventListener
{
  void callback( boost::any& sender )
  {
    IOUT( "EventListener::callback()" );
  }
};

TEST(eventmanager_listener)
{
  lost::event::EventManager                          manager;
  boost::shared_ptr<lost::event::EventManagerSignal> server(new lost::event::EventManagerSignal());
  boost::shared_ptr<EventListener>                   listener(new EventListener());
  boost::signals::connection                         listenerConnection;

  manager.registerEventServer( lost::event::UndefinedEventType, server );
  listenerConnection = manager.registerEventListener( lost::event::UndefinedEventType, boost::bind( &EventListener::callback, listener, _1 ) );

  boost::any any = listener;
  (*server)( any );

  CHECK(manager.signals.size() == 1);
  CHECK(listenerConnection.connected());
}

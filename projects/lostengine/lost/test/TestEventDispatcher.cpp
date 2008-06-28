#include <boost/bind.hpp>
#include "lost/event/EventDispatcher.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/event/KeyEvent.h"
#include <boost/function.hpp>

using namespace lost::event;

TEST(eventmanager)
{
  lost::event::EventDispatcher dispatcher;
  CHECK(dispatcher.listeners.size() == 0);
}

struct EventListener
{
  bool* dest;
  EventListener(bool* inDest) : dest(inDest) {}
  void operator()(EventPtr event)
  {
    IOUT("EventListener fired for : "+ event->type);
    (*dest) = true;
  }
};

TEST(eventmanager_listener)
{
  lost::event::EventDispatcher  dispatcher;
  bool signalFired = false;

  dispatcher.addEventListener( lost::event::KeyEvent::KEY_DOWN, EventListener(&signalFired));
  dispatcher.dispatchEvent(EventPtr(new KeyEvent(KeyEvent::KEY_DOWN)));

  CHECK(dispatcher.listeners.size() == 1);
  CHECK(signalFired);
}

#include <boost/bind.hpp>
#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/event/Receive.h"
#include <boost/function.hpp>

using namespace lost::event;
using namespace lost::application;
using namespace boost;

TEST(eventmanager)
{
  lost::event::EventDispatcher dispatcher;
  CHECK(dispatcher.numListeners() == 0);
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
  EventDispatcher  dispatcher;
  bool signalFired = false;

  dispatcher.addEventListener( lost::application::KeyEvent::KEY_DOWN(), EventListener(&signalFired));
  dispatcher.dispatchEvent(EventPtr(new KeyEvent(KeyEvent::KEY_DOWN())));

  CHECK(dispatcher.numListeners() == 1);
  CHECK(signalFired);
}

struct MouseListener
{
  bool* dest;
  MouseListener(bool* inDest) : dest(inDest) {}
  void operator()(MouseEventPtr event)
  {
    IOUT("EventListener fired for : "+ event->type);
    (*dest) = true;
  }
};

TEST(event_cast)
{
  EventDispatcher  dispatcher;
  bool signalFired = false;

  dispatcher.addEventListener( MouseEvent::MOUSE_DOWN(), receive<MouseEvent>(MouseListener(&signalFired)));
  dispatcher.dispatchEvent(EventPtr(new MouseEvent(MouseEvent::MOUSE_DOWN())));

  CHECK(dispatcher.numListeners() == 1);
  CHECK(signalFired);
}

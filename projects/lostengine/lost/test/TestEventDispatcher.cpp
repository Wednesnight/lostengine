#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/event/Listener.h"

using namespace lost::event;
using namespace lost::application;
using namespace boost;

TEST(eventmanager)
{
  lost::event::EventDispatcher dispatcher;
  CHECK(dispatcher.numListeners() == 0);
}

struct EventListener : Listener
{
  bool* dest;
  EventListener(bool* inDest) : dest(inDest) {}
  void call(const EventPtr& event)
  {
    IOUT("EventListener fired for : "<< event->type);
    (*dest) = true;
  }
};

TEST(eventmanager_listener)
{
  EventDispatcher  dispatcher;
  bool signalFired = false;

  dispatcher.addEventListener( lost::application::KeyEvent::KEY_DOWN(), ListenerPtr(new EventListener(&signalFired)));
  dispatcher.dispatchEvent(EventPtr(new KeyEvent(KeyEvent::KEY_DOWN())));

  CHECK(dispatcher.numListeners() == 1);
  CHECK(signalFired);
}

struct MouseListener : public Listener
{
  bool* dest;
  MouseListener(bool* inDest) : dest(inDest) {}
  void call(const EventPtr& event)
  {
    IOUT("EventListener fired for : "<< event->type);
    (*dest) = true;
  }
};

TEST(event_cast)
{
  EventDispatcher  dispatcher;
  bool signalFired = false;

  dispatcher.addEventListener( MouseEvent::MOUSE_DOWN(), ListenerPtr(new MouseListener(&signalFired)));
  dispatcher.dispatchEvent(EventPtr(new MouseEvent(MouseEvent::MOUSE_DOWN())));

  CHECK(dispatcher.numListeners() == 1);
  CHECK(signalFired);
}

/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/event/Event.h"
#include "lost/event/EventDispatcher.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/event/Listener.h"

using namespace lost::event;
using namespace lost::application;

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

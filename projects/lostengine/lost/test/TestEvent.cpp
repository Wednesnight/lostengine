#include "lost/event/Event.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"

using namespace lost::event;
using namespace lost::application;

TEST(event)
{
  lost::event::Event event("asd");
  CHECK(event.type == "asd");
}

TEST(keyevent)
{
  KeyEvent event(KeyEvent::KEY_DOWN);
  CHECK(event.type == KeyEvent::KEY_DOWN);
}

TEST(mouseevent)
{
  MouseEvent event(MouseEvent::MOUSE_DOWN);
  CHECK(event.type == MouseEvent::MOUSE_DOWN);
}

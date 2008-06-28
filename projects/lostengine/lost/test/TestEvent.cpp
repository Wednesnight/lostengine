#include "lost/event/Event.h"
#include "lost/event/KeyEvent.h"
#include "lost/event/MouseEvent.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"

using namespace lost::event;

TEST(event)
{
  lost::event::Event event("asd");
  CHECK(event.type == "asd");
}

TEST(keyevent)
{
  lost::event::KeyEvent event(KeyEvent::KEY_DOWN);
  CHECK(event.type == KeyEvent::KEY_DOWN);
}

TEST(mouseevent)
{
  lost::event::MouseEvent event(MouseEvent::MOUSE_DOWN);
  CHECK(event.type == MouseEvent::MOUSE_DOWN);
}

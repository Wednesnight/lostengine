#include "lost/event/Event.h"
#include "lost/event/KeyEvent.h"
#include "lost/event/MouseEvent.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"

TEST(event)
{
  lost::event::Event event;
  event.type = lost::event::UndefinedEventType;
  CHECK(event.type == lost::event::UndefinedEventType);
}

TEST(keyevent)
{
  lost::event::KeyEvent event;
  CHECK(event.type == lost::event::KeyEventType);
}

TEST(mouseevent)
{
  lost::event::MouseEvent event;
  CHECK(event.type == lost::event::MouseEventType);
}

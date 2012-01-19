#include "lost/event/Event.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/ResizeEvent.h"
#include "UnitTest++.h"
#include "lost/common/Logger.h"
#include "lost/common/Hash.h"

using namespace lost::event;
using namespace lost::application;

TEST(event)
{
  lost::event::Event event(lost::common::djb2Hash("asd"));
  CHECK(event.type == lost::common::djb2Hash("asd"));
}

TEST(keyevent)
{
  KeyEvent event(KeyEvent::KEY_DOWN());
  CHECK(event.type == KeyEvent::KEY_DOWN());
}

TEST(mouseevent)
{
  MouseEvent event(MouseEvent::MOUSE_DOWN());
  CHECK(event.type == MouseEvent::MOUSE_DOWN());
}

TEST(resizeevent)
{
  ResizeEvent event(ResizeEvent::TASKLET_WINDOW_RESIZE());
  CHECK(event.type == ResizeEvent::TASKLET_WINDOW_RESIZE());  
}

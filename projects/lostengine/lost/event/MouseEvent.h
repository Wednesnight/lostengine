#ifndef LOST_EVENT_MOUSEEVENT_H
#define LOST_EVENT_MOUSEEVENT_H

#include "lost/event/Event.h"
#include "lost/math/Vec2.h"

namespace lost
{
namespace event
{
  struct MouseEvent : public Event
  {
    lost::math::Vec2 pos;
    int              button;
    bool             pressed;

    MouseEvent() { type = lost::event::MouseEventType; }
    virtual ~MouseEvent() {}
  };

}
}

#endif

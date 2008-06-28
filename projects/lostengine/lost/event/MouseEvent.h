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
    static const EventType MOUSE_UP;
    static const EventType MOUSE_DOWN;
    static const EventType MOUSE_MOVE;
  
    lost::math::Vec2 pos;
    int              button;
    bool             pressed;

    MouseEvent(const EventType& inType) : Event(inType) { }
    virtual ~MouseEvent() {}
  };

}
}

#endif

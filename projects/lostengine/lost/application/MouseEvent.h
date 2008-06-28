#ifndef LOST_APPLICATION_MOUSEEVENT_H
#define LOST_APPLICATION_MOUSEEVENT_H

#include "lost/event/Event.h"
#include "lost/math/Vec2.h"

namespace lost
{
namespace application
{
  struct MouseEvent : public event::Event
  {
    static const event::Type MOUSE_UP;
    static const event::Type MOUSE_DOWN;
    static const event::Type MOUSE_MOVE;
  
    lost::math::Vec2 pos;
    int              button;
    bool             pressed;

    MouseEvent(const event::Type& inType) : Event(inType) { }
    virtual ~MouseEvent() {}
  };

}
}

#endif

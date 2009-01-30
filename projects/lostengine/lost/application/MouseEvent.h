#ifndef LOST_APPLICATION_MOUSEEVENT_H
#define LOST_APPLICATION_MOUSEEVENT_H

#include "lost/application/InputEvent.h"
#include "lost/math/Vec2.h"
#include "lost/application/MouseButton.h"

namespace lost
{
  namespace application
  {
    struct MouseEvent : public InputEvent
    {
      static const event::Type& MOUSE_UP()   { static const event::Type d = "mouseUp"; return d; }
      static const event::Type& MOUSE_DOWN() { static const event::Type d = "mouseDown"; return d; }
      static const event::Type& MOUSE_MOVE() { static const event::Type d = "mouseMove"; return d; }
    
      lost::math::Vec2 pos;
      lost::math::Vec2 absPos;
      MouseButton      button;
      bool             pressed;

      MouseEvent(const event::Type& inType) : InputEvent(inType), button(MB_UNKNOWN), pressed(false) {}
      virtual ~MouseEvent() {}
    };

  }
}

#endif

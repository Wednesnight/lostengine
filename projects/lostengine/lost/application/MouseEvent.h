#ifndef LOST_APPLICATION_MOUSEEVENT_H
#define LOST_APPLICATION_MOUSEEVENT_H

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/application/MouseButton.h"
#include "lost/event/Event.h"
#include "lost/common/Hash.h"

namespace lost
{
  namespace application
  {
    struct MouseEvent : public event::Event
    {
      static const event::Type& MOUSE_UP()   { static const event::Type d = common::djb2Hash("mouseUp"); return d; }
      static const event::Type& MOUSE_DOWN() { static const event::Type d = common::djb2Hash("mouseDown"); return d; }
      static const event::Type& MOUSE_MOVE() { static const event::Type d = common::djb2Hash("mouseMove"); return d; }
      static const event::Type& MOUSE_SCROLL() { static const event::Type d = common::djb2Hash("mouseScroll"); return d; }
    
      Window*          window;
      lost::math::Vec2 pos;
      lost::math::Vec2 absPos;
      MouseButton      button;
      bool             pressed;
      lost::math::Vec3 scrollDelta;

      MouseEvent(const event::Type& inType) : Event(inType), button(MB_UNKNOWN), pressed(false) {}
      virtual ~MouseEvent() {}
    };

  }
}

#endif

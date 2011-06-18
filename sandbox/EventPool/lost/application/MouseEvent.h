#ifndef LOST_APPLICATION_MOUSEEVENT_H
#define LOST_APPLICATION_MOUSEEVENT_H

//#include "lost/math/Vec2.h"
//#include "lost/math/Vec3.h"
#include "lost/application/MouseButton.h"

namespace lost
{
  namespace application
  {
    struct MouseEvent;
    typedef lost::shared_ptr<MouseEvent> MouseEventPtr;

    struct MouseEvent : public event::Event
    {
      static const event::Type& MOUSE_UP()   { static const event::Type d = "mouseUp"; return d; }
      static const event::Type& MOUSE_DOWN() { static const event::Type d = "mouseDown"; return d; }
      static const event::Type& MOUSE_MOVE() { static const event::Type d = "mouseMove"; return d; }
      static const event::Type& MOUSE_SCROLL() { static const event::Type d = "mouseScroll"; return d; }
    
//      lost::math::Vec2 pos;
//      lost::math::Vec2 absPos;
      MouseButton      button;
      bool             pressed;
//      lost::math::Vec3 scrollDelta;

      MouseEvent(const event::Type& inType) : Event(inType), button(MB_UNKNOWN), pressed(false) {}
      virtual ~MouseEvent() {}
    };

  }
}

#endif

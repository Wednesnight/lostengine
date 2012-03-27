/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

      static const event::Type& MOUSE_UP_INSIDE()  { static const event::Type d = common::djb2Hash("mouseUpInside"); return d; }
      static const event::Type& MOUSE_UP_OUTSIDE() { static const event::Type d = common::djb2Hash("mouseUpOutside"); return d; }
      static const event::Type& MOUSE_ENTER() { static const event::Type d = common::djb2Hash("mouseEnter"); return d; }
      static const event::Type& MOUSE_LEAVE() { static const event::Type d = common::djb2Hash("mouseLeave"); return d; }
    
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

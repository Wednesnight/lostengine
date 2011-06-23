#ifndef LOST_APPLICATION_KEYEVENT_H
#define LOST_APPLICATION_KEYEVENT_H

#include "lost/application/forward.h"
#include "lost/application/KeyCode.h"
#include "lost/event/Event.h"
#include "lost/common/Hash.h"

namespace lost
{
  namespace application
  {
    struct KeyEvent : public event::Event
    {
      static const event::Type& KEY_DOWN() { static const event::Type d = common::djb2Hash("keyDown"); return d; }
      static const event::Type& KEY_UP() { static const event::Type d = common::djb2Hash("keyUp"); return d; }
    
      Window*     window;
      KeyCode     key;
      string character; // FIXME: this will work for the moment, replace with more efficient type
      bool        pressed;
      bool        repeat;
      bool        ctrlDown;
      bool        shiftDown;
      bool        altDown;
      bool        specialDown;

      KeyEvent(const event::Type inType)
      : Event(inType),
        key(K_UNKNOWN),
        pressed(false),
        repeat(false),
        ctrlDown(false),
        shiftDown(false),
        altDown(false),
        specialDown(false)
      {}

      virtual ~KeyEvent() {}
    };

  }
}

#endif

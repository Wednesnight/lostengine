#ifndef LOST_APPLICATION_KEYEVENT_H
#define LOST_APPLICATION_KEYEVENT_H

#include "lost/event/Event.h"
#include "lost/application/KeyCode.h"

namespace lost
{
  namespace application
  {
    struct KeyEvent : public Event
    {
      static const event::Type& KEY_DOWN() { static const event::Type d = "keyDown"; return d; }
      static const event::Type& KEY_UP() { static const event::Type d = "keyUp"; return d; }
    
      KeyCode     key;
      eastl::string character; // FIXME: this will work for the moment, replace with more efficient type
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

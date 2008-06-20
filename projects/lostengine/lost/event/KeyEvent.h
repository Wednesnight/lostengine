#ifndef LOST_EVENT_KEYEVENT_H
#define LOST_EVENT_KEYEVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace event
{

  struct KeyEvent : public Event
  {
    int key;
    bool pressed;

    KeyEvent() { type = lost::event::KeyEventType; }
    virtual ~KeyEvent() {}
  };

}
}


#endif

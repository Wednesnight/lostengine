#ifndef LOST_EVENT_KEYEVENT_H
#define LOST_EVENT_KEYEVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace event
{

  struct KeyEvent : public Event
  {
    static const EventType KEY_DOWN;
    static const EventType KEY_UP;
  
    int key;
    bool pressed;

    KeyEvent(const EventType inType) : Event(inType) { }
    virtual ~KeyEvent() {}
  };

}
}


#endif

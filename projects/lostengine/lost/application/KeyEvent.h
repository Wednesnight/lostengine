#ifndef LOST_APPLICATION_KEYEVENT_H
#define LOST_APPLICATION_KEYEVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace application
{

  struct KeyEvent : public event::Event
  {
    static const event::Type& KEY_DOWN()
    {
      static const event::Type d = "keyDown";
      return d;    
    }
    
    static const event::Type& KEY_UP()
    {
      static const event::Type d = "keyUp";
      return d;
    }
  
    int  key;
    char character;
    bool pressed;
    bool repeat;

    KeyEvent(const event::Type inType) : Event(inType), key(0), pressed(false) { }
    virtual ~KeyEvent() {}
  };

}
}


#endif

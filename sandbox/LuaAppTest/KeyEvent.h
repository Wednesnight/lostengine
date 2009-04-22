#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "Event.h"

struct KeyEvent : public Event
{
  static const std::string KEY_DOWN;
  static const std::string KEY_UP;

  KeyEvent(const std::string& inType) : Event(inType) {}
  virtual ~KeyEvent() {}
  
  static lost::shared_ptr<KeyEvent> createKeyEvent(const std::string inType, unsigned long inKeyCode)
  {
    lost::shared_ptr<KeyEvent> result =  lost::shared_ptr<KeyEvent>(new KeyEvent(inType));
    result->keycode = inKeyCode;
    return result;
  }

  static lost::shared_ptr<KeyEvent> createKeyUpEvent(unsigned long inKeyCode)
  {
    return createKeyEvent(KEY_UP, inKeyCode);
  }

  static lost::shared_ptr<KeyEvent> createKeyDownEvent(unsigned long inKeyCode)
  {
    return createKeyEvent(KEY_DOWN, inKeyCode);
  }
  
  unsigned long keycode;
};

#endif

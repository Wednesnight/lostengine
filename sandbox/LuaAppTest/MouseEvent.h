#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "Event.h"
#include <boost/shared_ptr.hpp>

struct MouseEvent : public Event
{
  static const std::string MOUSE_DOWN;
  static const std::string MOUSE_UP;
  static const std::string MOUSE_MOVE;

  MouseEvent(const std::string& inType) : Event(inType) {}
  virtual ~MouseEvent() {}

  static boost::shared_ptr<MouseEvent> createMouseEvent(const std::string inType)
  {
    boost::shared_ptr<MouseEvent> result =  boost::shared_ptr<MouseEvent>(new MouseEvent(inType));
    return result;
  }

  static boost::shared_ptr<MouseEvent> createMouseDownEvent()
  {
    return createMouseEvent(MOUSE_DOWN);
  }

  static boost::shared_ptr<MouseEvent> createMouseUpEvent()
  {
    return createMouseEvent(MOUSE_UP);
  }

  static boost::shared_ptr<MouseEvent> createMouseMoveEvent()
  {
    return createMouseEvent(MOUSE_MOVE);
  }
  
  unsigned long x;
  unsigned long y;
};

#endif
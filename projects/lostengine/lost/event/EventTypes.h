#ifndef LOST_EVENT_EVENTTYPES_H
#define LOST_EVENT_EVENTTYPES_H

#include <boost/cstdint.hpp>

namespace lost
{
  namespace event
  {
    typedef boost::int32_t EventType;

    static const EventType UndefinedEventType = 0;

    static const EventType KeyEventType = 1;
    static const EventType KeyUpEventType = 2;
    static const EventType KeyDownEventType = 3;

    static const EventType MouseEventType = 4;
    static const EventType MouseMoveEventType = 5;
    static const EventType MouseUpEventType = 6;  // mouse button releases
    static const EventType MouseDownEventType = 7;  // mouse button pressed
    static const EventType MouseButtonEventType = 8; // someone fiddled the buttons
  }
}

#endif

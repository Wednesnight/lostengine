#ifndef LOST_APPLICATION_INPUTEVENT_H
#define LOST_APPLICATION_INPUTEVENT_H

#include "lost/event/Event.h"
#include "lost/application/Window.h"

namespace lost
{
  namespace application
  {
    struct InputEvent : public event::Event
    {
      boost::shared_ptr<Window> window;

      InputEvent(const event::Type& inType) : Event(inType) {}
      virtual ~InputEvent() {}
    };

  }
}

#endif

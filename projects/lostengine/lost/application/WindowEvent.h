#ifndef LOST_APPLICATION_WINDOWEVENT_H
#define LOST_APPLICATION_WINDOWEVENT_H

#include "lost/event/Event.h"
#include "lost/application/Window.h"

namespace lost
{
  namespace application
  {
    struct WindowEvent;
    typedef lost::shared_ptr<WindowEvent> WindowEventPtr;

    struct WindowEvent : public event::Event
    {
      static const event::Type& CLOSE() { static event::Type d = "windowClose"; return d; }

      Window* window;

      WindowEvent(const event::Type& inType, Window* inWindow) : Event(inType), window(inWindow) {}
      virtual ~WindowEvent() {}

      static WindowEventPtr create(const event::Type& inType, Window* inWindow) { return WindowEventPtr(new WindowEvent(inType, inWindow)); }
    };
  }
}

#endif

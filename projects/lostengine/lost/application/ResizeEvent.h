#ifndef LOST_APPLICATION_RESIZEEVENT_H
#define LOST_APPLICATION_RESIZEEVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace application
{
  struct ResizeEvent;
  typedef lost::shared_ptr<ResizeEvent> ResizeEventPtr;

  /** sent when main application window size changes.
   */
  struct ResizeEvent : public event::Event
  {
    static const event::Type& MAIN_WINDOW_RESIZE()
    {
      static event::Type d = "mainWindowResize";
      return d;
    }
    
    int width;
    int height;

    ResizeEvent(const event::Type& inType) : Event(inType), width(0), height(0) { }
    ResizeEvent(int w, int h) : Event(MAIN_WINDOW_RESIZE()), width(w), height(h) {}
    virtual ~ResizeEvent() {}
  };

}
}


#endif

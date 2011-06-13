#ifndef LOST_APPLICATION_RESIZEEVENT_H
#define LOST_APPLICATION_RESIZEEVENT_H

#include "lost/event/forward.h"
#include "lost/event/Event.h"

namespace lost
{
namespace application
{
  /** sent when tasklet window size changes.
   */
  struct ResizeEvent : public event::Event
  {
    static const event::Type& TASKLET_WINDOW_RESIZE() { static event::Type d = "taskletWindowResize"; return d; }
    
    int width;
    int height;

    ResizeEvent(const event::Type& inType) : Event(inType), width(0), height(0) { }
    ResizeEvent(int w, int h) : Event(TASKLET_WINDOW_RESIZE()), width(w), height(h) {}
    virtual ~ResizeEvent() {}

    static ResizeEventPtr create(int w, int h) { return ResizeEventPtr(new ResizeEvent(w, h)); }
  };

}
}


#endif

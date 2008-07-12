#ifndef LOST_APPLICATION_RESIZEEVENT_H
#define LOST_APPLICATION_RESIZEEVENT_H

#include "lost/event/Event.h"

namespace lost
{
namespace application
{
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

    ResizeEvent(const event::Type& inType) : Event(inType) { }
    virtual ~ResizeEvent() {}
  };

}
}


#endif

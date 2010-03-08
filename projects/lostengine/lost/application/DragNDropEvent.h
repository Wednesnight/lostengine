#ifndef LOST_APPLICATION_DRAGNDROPEVENT_H
#define LOST_APPLICATION_DRAGNDROPEVENT_H

#include "lost/event/Event.h"
#include "lost/application/Window.h"
#include "lost/math/Vec2.h"

namespace lost
{
  template <typename T>
  class shared_ptr;
  namespace application
  {
    struct DragNDropEvent;
    typedef lost::shared_ptr<DragNDropEvent> DragNDropEventPtr;  
    
    struct DragNDropEvent : public event::Event
    {
      std::string filename;
      Window* window;
      lost::math::Vec2 pos;
      lost::math::Vec2 absPos;

      static const event::Type& DRAG_ENTER() { static const event::Type d = "dragEnter"; return d; }
      static const event::Type& DRAG_UPDATE() { static const event::Type d = "dragUpdate"; return d; }
      static const event::Type& DRAG_LEAVE() { static const event::Type d = "dragLeave"; return d; }
      static const event::Type& DROP() { static event::Type d = "drop"; return d; }
      
      DragNDropEvent(const event::Type& inType, const std::string& inFilename) : Event(inType), filename(inFilename) {}
      virtual ~DragNDropEvent() {}
      
    };
    
  }
}

#endif

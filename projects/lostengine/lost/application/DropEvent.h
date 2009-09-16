#ifndef LOST_APPLICATION_DROPEVENT_H
#define LOST_APPLICATION_DROPEVENT_H

#include "lost/event/Event.h"
#include "lost/application/Window.h"
#include "lost/math/Vec2.h"

namespace lost
{
  template <typename T>
  struct shared_ptr;
  namespace application
  {
    struct DropEvent;
    typedef lost::shared_ptr<DropEvent> DropEventPtr;  
    
    struct DropEvent : public event::Event
    {
      std::string filename;
      Window* window;
      lost::math::Vec2 pos;
      lost::math::Vec2 absPos;

      static const event::Type& DROPPED_FILE() { static event::Type d = "droppedFile"; return d; }
      
      DropEvent(const std::string& inFilename) : Event(DROPPED_FILE()), filename(inFilename) {}
      virtual ~DropEvent() {}
      
    };
    
  }
}

#endif

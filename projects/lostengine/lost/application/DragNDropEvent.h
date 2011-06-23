#ifndef LOST_APPLICATION_DRAGNDROPEVENT_H
#define LOST_APPLICATION_DRAGNDROPEVENT_H

#include "lost/event/Event.h"
#include "lost/application/Window.h"
#include "lost/math/Vec2.h"
#include "lost/common/Hash.h"

namespace lost
{
  namespace application
  {
    struct DragNDropEvent : public event::Event
    {
      vector<string> paths;
      Window* window;
      lost::math::Vec2 pos;
      lost::math::Vec2 absPos;

      uint32_t numPaths() { return paths.size(); }
      void clearPaths() {paths.clear(); }
      void addPath(const string& inPath) { paths.push_back(inPath); }
      string getPath(uint32_t idx) 
      {
        return paths[idx]; 
      };

      static const event::Type& DRAG_ENTER() { static const event::Type d = common::djb2Hash("dragEnter"); return d; }
      static const event::Type& DRAG_UPDATE() { static const event::Type d = common::djb2Hash("dragUpdate"); return d; }
      static const event::Type& DRAG_LEAVE() { static const event::Type d = common::djb2Hash("dragLeave"); return d; }
      static const event::Type& DROP() { static event::Type d = common::djb2Hash("drop"); return d; }
      
      DragNDropEvent(const event::Type& inType) : Event(inType) {}
      virtual ~DragNDropEvent() {}
      
    };
    
  }
}

#endif

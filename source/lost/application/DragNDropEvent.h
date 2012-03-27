/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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

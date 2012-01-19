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

#ifndef LOST_APPLICATION_RESIZEEVENT_H
#define LOST_APPLICATION_RESIZEEVENT_H

#include "lost/application/forward.h"
#include "lost/event/Event.h"
#include "lost/common/Hash.h"

namespace lost
{
namespace application
{
  /** sent when tasklet window size changes.
   */
  struct ResizeEvent : public event::Event
  {
    static const event::Type& TASKLET_WINDOW_RESIZE() { static event::Type d = common::djb2Hash("taskletWindowResize"); return d; }
    
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

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

#ifndef LOST_APPLICATION_WINDOW_H
#define LOST_APPLICATION_WINDOW_H

#include "lost/gl/forward.h"
#include "lost/event/forward.h"
#include "lost/application/forward.h"
#include "lost/math/Vec2.h"

namespace lost
{
  namespace application
  {
    
    struct Window
    {
    private:
      /**
       * forward declaration for platform specific stuff
       */
      struct WindowHiddenMembers;
      WindowHiddenMembers* hiddenMembers;

      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

    public:
      /**
       * the target for all events received by the window (typically key/mouse events)
       */
      // FIXME: should be private
      event::EventDispatcherPtr dispatcher;
      /**
       * generic interface to this window's OpenGL context
       */
      gl::ContextPtr context;

      TaskletConfig* config;
      
      Window(const event::EventDispatcherPtr& inDispatcher, TaskletConfig* inConfig);
      ~Window();

      /**
       * window is shown and moved to the application top-level
       */
      // TODO: open event
      void open();

      /**
       * closes the window, doesn't cleanup! after invoking this we're still able to call open() again
       */
      // TODO: close event (wait for the new dispatcher machanism since this only makes sense when we're able to cancel the process)
      void close();
      
      math::Vec2 size; // current size of the window
    };

  }
}

#endif

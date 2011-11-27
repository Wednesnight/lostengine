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

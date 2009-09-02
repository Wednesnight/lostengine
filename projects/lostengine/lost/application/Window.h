#ifndef LOST_APPLICATION_WINDOW_H
#define LOST_APPLICATION_WINDOW_H

#include "lost/gl/Context.h"
#include "lost/math/Rect.h"
#include "lost/event/EventDispatcher.h"

namespace lost
{
  namespace application
  {
    
    struct WindowParams
    {
    public:
      std::string      caption;
      lost::math::Rect rect;

      WindowParams()
      : caption("<unset>"), rect(0,0,640,480)
      {
      }

      WindowParams(const std::string& inCaption,
                   const lost::math::Rect& inRect)
      : caption(inCaption),
        rect(inRect)
      {
      }
    };

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
      lost::shared_ptr<lost::event::EventDispatcher> dispatcher;
      /**
       * generic interface to the window's OpenGL context
       */
      // FIXME: should probably be read-only
      lost::shared_ptr<gl::Context> context;

      /**
       * holds generic window params
       */
      WindowParams params;
      
      Window(const lost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
             const WindowParams& inParams);
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
    };

  }
}

#endif

#ifndef LOST_APPLICATION_WINDOW_H
#define LOST_APPLICATION_WINDOW_H

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "lost/application/gl/Canvas.h"
#include "lost/application/gl/Context.h"
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

      WindowParams(const std::string& inCaption,
                   const lost::math::Rect& inRect)
      : caption(inCaption),
        rect(inRect)
      {
      }
    };

    struct Window : public boost::enable_shared_from_this<Window>
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

      /**
       * don't use ctors directly! leave them private since we need to be held by a boost::shared_ptr,
       * otherwise shared_from_this() will fail!
       */
      Window(const boost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
             const WindowParams& inParams);
    public:
      /**
       * the target for all events received by the window (typically key/mouse events)
       */
      // FIXME: should be private
      boost::shared_ptr<lost::event::EventDispatcher> dispatcher;
      /**
       * generic interface to the window's OpenGL context
       */
      // FIXME: should probably be read-only
      boost::shared_ptr<gl::Context> context;

      /**
       * canvas for drawing operations
       */
      // FIXME: should probably be read-only
      boost::shared_ptr<gl::Canvas> canvas;
      
      /**
       * holds generic window params
       */
      WindowParams params;
      
      /**
       * static ctor helpers, make sure that we're held by a boost::shared_ptr
       */
      static boost::shared_ptr<Window> create(const boost::shared_ptr<lost::event::EventDispatcher>& inDispatcher,
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

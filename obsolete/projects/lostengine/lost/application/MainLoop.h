#ifndef LOST_APPLICATION_MAINLOOP_H
#define LOST_APPLICATION_MAINLOOP_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/event/Event.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace application
  {
    struct MainLoop
    {
      bool running;
      boost::shared_ptr<gl::Context> context;
      
      MainLoop(const boost::shared_ptr<gl::Context>& inContext)
      : context(inContext)
      {
      }

      void quitEventHandler(boost::shared_ptr<event::Event> event)
      {
        running = false;
      }

      virtual void process()
      {
      }

      void run()
      {
        context->makeCurrent();
        running = true;
        DOUT("starting mainLoop");
        while(running)
        {
          process();
        }
        DOUT("stopping mainLoop");
      }    
    };


    struct FunctorMainLoop : public MainLoop
    {
      boost::function<void (void)> func;

      FunctorMainLoop(const boost::shared_ptr<gl::Context>& inContext, const boost::function<void (void)>& f)
      : MainLoop(inContext),
        func(f)
      {
      }

      virtual void process()
      {
        func();
      }
    };
    
    
    
  }
}

#endif


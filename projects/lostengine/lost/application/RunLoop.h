#ifndef LOST_APPLICATION_RUNLOOP_H
#define LOST_APPLICATION_RUNLOOP_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "lost/event/Event.h"

namespace lost
{
  namespace application
  {
        
    struct RunLoop
    {
      bool running;
      
      void quitEventHandler(boost::shared_ptr<lost::event::Event> event)
      {
        running = false;
      }
      
      virtual void process()
      {
      }
      
      void run()
      {
        running = true;
        while(running)
        {
          process();
        }
      }    
    };


    struct RunLoopFunctor : public RunLoop
    {
      boost::function<void (void)> function;
      
      RunLoopFunctor(const boost::function<void (void)>& inFunction)
      : function(inFunction)
      {
      }
      
      virtual void process()
      {
        function();
      }
    };

    struct RunLoopScript : public RunLoop
    {
      boost::filesystem::path filename;
      
      RunLoopScript(const boost::filesystem::path& inFilename)
      : filename(inFilename)
      {
      }
      
      virtual void process()
      {
        // TODO: run interpreter
      }
    };
    
  }
}

#endif

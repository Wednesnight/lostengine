#ifndef LOST_APPLICATION_RUNLOOP_H
#define LOST_APPLICATION_RUNLOOP_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/signal.hpp>
#include "lost/event/Event.h"
#include "lost/common/Logger.h"
#include "lost/lua/State.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/lua/BindAll.h"

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

      boost::shared_ptr<lost::resource::Loader>  loader;
      boost::shared_ptr<lost::lua::State>        interpreter;
      
      RunLoopScript(const boost::filesystem::path& inFilename)
      : filename(inFilename)
      {
        loader.reset(new lost::resource::DefaultLoader);              // init default resource loader
        interpreter.reset(new lua::State(true, true, true, loader));  // init lua state with resource loader
        lost::lua::bindAll(*interpreter);                             // bind lostengine lua mappings    
        
        luabind::globals(*interpreter)["lost"]["globals"]          = luabind::newtable(*interpreter);   // create globals table
        luabind::globals(*interpreter)["lost"]["globals"]["state"] = interpreter;                       // map the state itself into the interpreter so scripts can use it
        
        lost::lua::ModuleLoader::install(*interpreter, loader); // install custom module loader so require goes through resourceLoader

      }
      
      virtual void process()
      {
        //
      }
    };
    
  }
}

#endif

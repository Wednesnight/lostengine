#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include "fhtagn/threads/tasklet.h"
#include <boost/enable_shared_from_this.hpp>
#include "lost/application/Config.h"
#include <list>
#include "lost/resource/DefaultLoader.h"

namespace lost
{
  namespace event {    struct EventDispatcher; }
  namespace lua { struct State; }
  namespace application
  {
    struct Application;
  
    struct Tasklet;
    typedef boost::shared_ptr<Tasklet> TaskletPtr;

    struct Tasklet : public fhtagn::threads::tasklet, public boost::enable_shared_from_this<Tasklet>
    {
    private:
      // internal flag to check if we have a valid script
      bool executeScript;

      void run(fhtagn::threads::tasklet& tasklet);
      bool callScriptFunction(const std::string& funcname); // calls the given function with tasklet as parameter

      void error(fhtagn::threads::tasklet& tasklet, std::exception const& exception);
    public:
      lost::resource::LoaderPtr         loader;
      lost::lua::StatePtr               interpreter;
      lost::application::ConfigPtr      config;
      lost::event::EventDispatcherPtr   eventDispatcher;
      lost::application::Application *  application;
      
      // if true, only runs the main loop once a low level event arrives
      bool waitForEvents;
      // script filename
      std::string script;

      Tasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
      virtual ~Tasklet();

      virtual bool startup();
      virtual bool main();
      virtual bool shutdown();

      virtual bool start();
      virtual bool stop();
    };

  }
}

#endif

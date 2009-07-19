#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include "fhtagn/threads/tasklet.h"
#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/State.h"

namespace lost
{
  namespace event {    struct EventDispatcher; }
  namespace application
  {
    struct Tasklet;
    typedef lost::shared_ptr<Tasklet> TaskletPtr;

    struct Window;
    typedef lost::shared_ptr<Window> WindowPtr;
  
    struct Tasklet : public fhtagn::threads::tasklet, public lost::enable_shared_from_this<Tasklet>
    {
    private:
      // internal flag to check if we have a valid script
      bool executeScript;

      void run(fhtagn::threads::tasklet& tasklet);
      bool callScriptFunction(const std::string& funcname); // calls the given function with tasklet as parameter

      void error(fhtagn::threads::tasklet& tasklet, std::exception const& exception);
    public:
      lost::resource::LoaderPtr       loader;
      lost::lua::StatePtr             lua;
      lost::event::EventDispatcherPtr eventDispatcher;
      WindowPtr                       window;
      
      // if true, only runs the main loop once a low level event arrives
      bool waitForEvents;
      // script filename
      std::string script;

      Tasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
      virtual ~Tasklet();

      // initializes resources on the tasklet thread
      virtual void init();

      virtual bool startup();
      virtual bool main();
      virtual bool shutdown();

      virtual bool start();
      virtual bool stop();
    };

  }
}

#endif

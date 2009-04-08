#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include "fhtagn/threads/tasklet.h"
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "lost/lua/State.h"
#include "lost/application/Config.h"
#include <list>
#include "lost/event/EventDispatcher.h"
#include "lost/resource/DefaultLoader.h"

namespace lost
{
  namespace application
  {
    struct Tasklet;
    typedef boost::shared_ptr<Tasklet> TaskletPtr;

    struct Tasklet : public fhtagn::threads::tasklet, public boost::enable_shared_from_this<Tasklet>
    {
    private:
      // internal flag to check if we have a valid script
      bool executeScript;

      void run(fhtagn::threads::tasklet& tasklet);
      bool runScript(const std::string& stage);

    public:
      lost::resource::LoaderPtr       loader;
      lost::lua::StatePtr             interpreter;
      lost::application::ConfigPtr    config;
      lost::event::EventDispatcherPtr eventDispatcher;
      
      // if true, only runs the main loop once a low level event arrives
      bool waitForEvents;
      // script filename
      std::string script;

      Tasklet(const lost::resource::LoaderPtr& inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
      virtual ~Tasklet();

      virtual bool startup();
      virtual bool main();
      virtual bool shutdown();
    };

  }
}

#endif

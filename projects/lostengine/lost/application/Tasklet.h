#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include "lost/application/Window.h"
#include "fhtagn/threads/tasklet.h"
#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/lua/State.h"

namespace lost
{
  namespace event { struct EventDispatcher; }
  namespace application
  {
    struct Tasklet;
    typedef lost::shared_ptr<Tasklet> TaskletPtr;

    struct Window;
    typedef lost::shared_ptr<Window> WindowPtr;
  
    struct Tasklet : public fhtagn::threads::tasklet, public lost::enable_shared_from_this<Tasklet>
    {
    private:
      bool scriptLoaded; // tru if 'main.lua' was successfully loaded

      void run(fhtagn::threads::tasklet& tasklet);

      void error(fhtagn::threads::tasklet& tasklet, std::exception const& exception);
      
      bool hasLuaStartup;
      luabind::object luaStartup;
      bool hasLuaUpdate;
      luabind::object luaUpdate;
      bool hasLuaShutdown;
      luabind::object luaShutdown;
      bool hasLuaProcessCallLater;
      luabind::object luaProcessCallLater;
      
    public:
      std::string                     scriptname;
      lost::resource::LoaderPtr       loader;
      lost::lua::StatePtr             lua;
      lost::event::EventDispatcherPtr eventDispatcher;
      WindowParams                    windowParams; // fill this structure with the necessary params if you want a window with GL context
      WindowPtr                       window; // contains the window pointer after init() if it could be created
      
      // if true, only runs the main loop once a low level event arrives
      bool waitForEvents;

      Tasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
      virtual ~Tasklet();

      virtual void init(); // runs on main thread: reads main.lua if present and creates a window if desired

      virtual bool startup(); // called once on worker thread, return false if startup fails
      virtual bool update(); // called repeatedly on worker thread, return false if you want to shutdown
      virtual bool shutdown(); // called once on worker thread, return false if shutdown failed

      virtual bool start(); 
      virtual bool stop();

      void queueApplicationEvent(event::EventPtr event);
      void dispatchApplicationEvent(event::EventPtr event);
      void processApplicationEvents();
    };

  }
}

#endif

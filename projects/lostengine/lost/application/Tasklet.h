#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include "fhtagn/threads/tasklet.h"
#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/application/WindowParams.h"
#include <luabind/luabind.hpp>

namespace lost
{
  namespace event 
  { 
    struct EventDispatcher; typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr;
    struct Event; typedef lost::shared_ptr<Event> EventPtr;    
  }
  namespace rg { struct Node; typedef lost::shared_ptr<Node> NodePtr; }
  namespace lua { struct State; typedef lost::shared_ptr<State> StatePtr; }
  namespace application
  {
    struct Queue;
    typedef lost::shared_ptr<Queue> QueuePtr;  
  
    struct Window;

    struct WindowEvent;
    typedef lost::shared_ptr<WindowEvent> WindowEventPtr;
  
    struct Tasklet : public fhtagn::threads::tasklet
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
      
    public:
      std::string                     name;
      resource::LoaderPtr             loader;
      lua::StatePtr                   lua;
      event::EventDispatcherPtr       eventDispatcher;
      WindowParams                    windowParams;   // fill this structure with the necessary params if you want a window with GL context
      Window*                         window;         // contains the window pointer after init() if it could be created
      rg::NodePtr                     renderNode;     // render graph root node
      QueuePtr                        updateQueue;    // queue that holds native/lua objects that should be updated within each loop run
      
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

      void closeWindow(WindowEventPtr event);

      void queueApplicationEvent(event::EventPtr event);
      void dispatchApplicationEvent(event::EventPtr event);
      void processApplicationEvents();
    };

  }
}

#endif

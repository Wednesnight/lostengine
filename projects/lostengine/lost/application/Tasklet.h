#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/application/WindowParams.h"
#include "lost/application/Queue.h"
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
    struct Window;

    struct WindowEvent;
    typedef lost::shared_ptr<WindowEvent> WindowEventPtr;
  
    struct Tasklet
    {
      // if true, only runs the main loop once a low level event arrives
      bool waitForEvents;    
      bool scriptLoaded; // tru if 'main.lua' was successfully loaded
      
      bool hasLuaStartup;
      luabind::object luaStartup;
      bool hasLuaUpdate;
      luabind::object luaUpdate;
      bool hasLuaShutdown;
      luabind::object luaShutdown;

      event::EventDispatcherPtr       eventDispatcher;    
      std::string                     name;
      resource::LoaderPtr             loader;
      lua::StatePtr                   lua;
      WindowParams                    windowParams;   // fill this structure with the necessary params if you want a window with GL context
      Window*                         window;         // contains the window pointer after init() if it could be created
      rg::NodePtr                     renderNode;     // render graph root node
      QueuePtr                        updateQueue;    // queue that holds native/lua objects that should be updated within each loop run
      

      Tasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
      virtual ~Tasklet();

      void queueApplicationEvent(event::EventPtr event);
      void dispatchApplicationEvent(event::EventPtr event);
      void processApplicationEvents();  

      virtual void init(); // runs on main thread: reads main.lua if present and creates a window if desired

      virtual bool startup(); // called once on worker thread, return false if startup fails
      virtual bool update(); // called repeatedly on worker thread, return false if you want to shutdown
      virtual bool shutdown(); // called once on worker thread, return false if shutdown failed

      void render();
      void processEvents();
      
      void createWindow(const WindowParams& params);
      void closeWindow(WindowEventPtr event);
      
      // this is the part of the interface that can be/is used for correct handling of multithreading
      // FIXME: hat are the semantics of these functions within a single threaded use case?
      virtual bool start() { return false; }; // override this, starts tasklet, potentially on separate thread
      virtual bool stop() { return false; }; // override this, tells tasklet to stop and shutdown
      virtual bool alive() { return false; } // override this to tell application if you're still alive, FIXME: what's the definition of this state?
      virtual bool wait() { return false; } // WTF?
      
    };

  }
}

#endif

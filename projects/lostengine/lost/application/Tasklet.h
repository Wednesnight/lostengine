#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/application/WindowParams.h"
#include "lost/application/Queue.h"
#include "lost/lua/lua.h"

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
	  protected:
      /**
       * forward declaration for platform specific stuff
       */
      struct TaskletHiddenMembers;
      lost::shared_ptr<TaskletHiddenMembers> hiddenMembers;

      bool isAlive;
      bool scriptLoaded; // true if 'main.lua' was successfully loaded

      bool hasLuaStartup;
      luabind::object luaStartup;
      bool hasLuaUpdate;
      luabind::object luaUpdate;
      bool hasLuaShutdown;
      luabind::object luaShutdown;

      lua::StatePtr                   lua;
      WindowParams                    windowParams;   // fill this structure with the necessary params if you want a window with GL context

      virtual void init();    // reads main.lua if present and creates a window if desired; called from platform specific implementation
      virtual void cleanup(); // cleanup all resources; called from platform specific implementation

  	  virtual bool startup();	  // called once in run, return false if startup fails
      virtual bool update();    // called repeatedly in run, return false if you want to shutdown
      virtual void render();    // called repeatedly in run
      virtual bool shutdown();  // called once in run, return false if shutdown failed

      void createWindow(const WindowParams& params);
      void closeWindow(WindowEventPtr event);
      
      void processEvents();

    public:
      bool waitForEvents; // if true, only runs the main loop once a low level event arrives
      
      event::EventDispatcherPtr       eventDispatcher;    
      std::string                     name;
      resource::LoaderPtr             loader;

	    Window*                         window;         // contains the window pointer after init() if it could be created
      rg::NodePtr                     renderNode;     // render graph root node
      QueuePtr                        updateQueue;    // queue that holds native/lua objects that should be updated within each loop run
      

      Tasklet(lost::resource::LoaderPtr inLoader = resource::DefaultLoader::create());
      virtual ~Tasklet();

      void queueApplicationEvent(event::EventPtr event);
      void dispatchApplicationEvent(event::EventPtr event);
      void processApplicationEvents();  

      bool alive();         // tell application if tasklet is still alive      

      void start(); // starts tasklet; platform specific implementation
      void run();   // the tasklet run loop; platform specific implementation
      void stop();  // tells tasklet to stop and shutdown; platform specific implementation
    };

  }
}

#endif

#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/application/WindowParams.h"
#include "lost/application/Queue.h"
#include <luabind/luabind.hpp>
#include "lost/application/MultiThreadedTasklet.h"

namespace lost
{
  namespace rg { struct Node; typedef lost::shared_ptr<Node> NodePtr; }
  namespace lua { struct State; typedef lost::shared_ptr<State> StatePtr; }
  namespace application
  {
    struct Window;

    struct WindowEvent;
    typedef lost::shared_ptr<WindowEvent> WindowEventPtr;
  
    struct Tasklet : public MultiThreadedTasklet
    {
    private:
      bool scriptLoaded; // tru if 'main.lua' was successfully loaded

      void run(fhtagn::threads::tasklet& tasklet);
      
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
      WindowParams                    windowParams;   // fill this structure with the necessary params if you want a window with GL context
      Window*                         window;         // contains the window pointer after init() if it could be created
      rg::NodePtr                     renderNode;     // render graph root node
      QueuePtr                        updateQueue;    // queue that holds native/lua objects that should be updated within each loop run
      

      Tasklet(lost::resource::LoaderPtr inLoader= lost::resource::LoaderPtr(new lost::resource::DefaultLoader));
      virtual ~Tasklet();

      virtual void init(); // runs on main thread: reads main.lua if present and creates a window if desired

      virtual bool startup(); // called once on worker thread, return false if startup fails
      virtual bool update(); // called repeatedly on worker thread, return false if you want to shutdown
      virtual bool shutdown(); // called once on worker thread, return false if shutdown failed

      void render();
      void processEvents();
      
      void createWindow(const WindowParams& params);
      void closeWindow(WindowEventPtr event);
    };

  }
}

#endif

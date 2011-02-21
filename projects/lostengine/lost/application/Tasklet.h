#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include <list>
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/DefaultWriter.h"
#include "lost/application/Queue.h"
#include "lost/event/forward.h"
#include "lost/application/forward.h"
#include "lost/lua/forward.h"
#include "lost/rg/forward.h"
#include "lost/application/TaskletConfig.h"
#include "lost/font/forward.h"
#include "lost/time/Clock.h"
#include "lost/time/TimerScheduler.h"

namespace lost
{
  namespace application
  {
    struct Tasklet
    {
	  protected:
      /**
       * forward declaration for platform specific stuff
       */
      struct TaskletHiddenMembers;
      lost::shared_ptr<TaskletHiddenMembers> hiddenMembers;

      struct LuaStateHelper;
      lost::shared_ptr<LuaStateHelper> lsh;

      time::Clock clock;

      bool isAlive;
      bool scriptLoaded; // true if 'main.lua' was successfully loaded
      bool configLoaded; // true if config.lua was successfully loaded

      bool hasLuaStartup;
      bool hasLuaUpdate;
      bool hasLuaShutdown;
      bool hasLuaKey;


      virtual void init();    // reads main.lua if present and creates a window if desired; called from platform specific implementation
      virtual void cleanup(); // cleanup all resources; called from platform specific implementation

  	  virtual bool startup();                       // called once in run, returns this->running
      virtual bool update(double deltaSeconds);     // called repeatedly in run, returns this->running
      virtual void render();                        // called repeatedly in run
      virtual void shutdown();                      // called once in run

      void createWindow();
      void closeWindow(WindowEventPtr event);
      
      void processEvents();
      void updateWindowSize(const application::ResizeEventPtr& event);
      

    public:
      lua::StatePtr                   lua;
      bool                            waitForEvents; // if true, only runs the main loop once a low level event arrives
      
      event::EventDispatcherPtr       eventDispatcher;    
      std::string                     name;
      resource::LoaderPtr             loader;
      resource::WriterPtr             writer;
      TaskletConfig                   config;
      font::FontManagerPtr            fontManager;
      lost::shared_ptr<time::TimerScheduler> scheduler;

	    Window*                         window;         // contains the window pointer after init() if it could be created
      rg::NodePtr                     clearNode;      // default clear node, always present
      rg::NodePtr                     renderNode;     // render graph root node
      rg::NodePtr                     uiNode;         // guiro.Screen will add its render nodes here
      QueuePtr                        updateQueue;    // queue that holds native/lua objects that should be updated within each loop run
      bool                            running;        // tasklet will shutdown if this flag is set to false

      Tasklet(lost::resource::LoaderPtr inLoader = resource::DefaultLoader::create(),
              lost::resource::WriterPtr inWriter = resource::DefaultWriter::create());
      virtual ~Tasklet();

      void queueApplicationEvent(event::EventPtr event);
      void dispatchApplicationEvent(event::EventPtr event);
      void processApplicationEvents();  

      bool alive();         // tell application if tasklet is still alive      

      void start(); // starts tasklet; platform specific implementation
      void run();   // the tasklet run loop; platform specific implementation
      void stop();  // tells tasklet to stop and shutdown; platform specific implementation
      
      std::string getClipboardString(); // platform specific implementation
      bool setClipboardString(const std::string& str); // platform specific implementation
      
      virtual void key(const application::KeyEventPtr& ev);
    };

  }
}

#endif

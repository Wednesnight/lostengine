/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_APPLICATION_TASKLET_H
#define LOST_APPLICATION_TASKLET_H

#include "lost/application/TaskletThread.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/application/Queue.h"
#include "lost/event/forward.h"
#include "lost/application/forward.h"
#include "lost/lua/forward.h"
#include "lost/rg/forward.h"
#include "lost/application/TaskletConfig.h"
#include "lost/font/forward.h"
#include "lost/time/Clock.h"
#include "lost/time/TimerScheduler.h"
#include "lost/resource/Bundle.h"

namespace lost
{
  namespace application
  {
    struct Tasklet
    {
	  protected:
      lost::shared_ptr<TaskletThread> thread;
      
      struct LuaStateHelper;
      lost::shared_ptr<LuaStateHelper> lsh;

      time::Clock clock;

      bool isAlive;
      bool scriptLoaded; // true if 'main.lua' was successfully loaded
      bool configLoaded; // true if config.lua was successfully loaded

  	  virtual bool startup();                       // called before starting runloop, returns this->running
      virtual bool update(double deltaSeconds);     // called repeatedly in runloop, returns this->running
      virtual void render();                        // called repeatedly in runloop
      virtual void shutdown();                      // called after finishing runloop

      void createWindow();
      void closeWindow(const WindowEventPtr& event);
      
      void processEvents();
      void updateWindowSize(const ResizeEventPtr& event);

      void handleDebugEvent(const DebugEventPtr& event);

    public:
      lua::StatePtr                   lua;
      bool                            waitForEvents; // if true, only runs the main loop once a low level event arrives
      
      event::EventDispatcherPtr       eventDispatcher;    
      string                     name;
      resource::LoaderPtr             loader;
      TaskletConfig                   config;
      font::FontManagerPtr            fontManager;
      lost::shared_ptr<time::TimerScheduler> scheduler;

	    Window*                         window;         // contains the window pointer after init() if it could be created
      rg::NodePtr                     clearNode;      // default clear node, always present
      rg::NodePtr                     renderNode;     // render graph root node
      rg::NodePtr                     uiNode;         // guiro.Screen will add its render nodes here
      QueuePtr                        updateQueue;    // queue that holds native/lua objects that should be updated within each loop run
      bool                            running;        // tasklet will shutdown if this flag is set to false

      resource::BundlePtr             applicationBundle;
      resource::BundlePtr             taskletBundle;
      
      Tasklet(lost::resource::LoaderPtr inLoader = resource::DefaultLoader::create());
      virtual ~Tasklet();

      // FIXME: EVIL! we need to get rid of these because if a tasklet creates these events and sends them to the application,
      // the events will be destroyed on the main thread which will force slub to access the Lua interpreter from the wrong 
      // thread, wreaking havoc. 
      void queueApplicationEvent(event::EventPtr event);
      void dispatchApplicationEvent(event::EventPtr event);
      void processApplicationEvents();  

      // use these specialised functions which will create the events without dragging Lua state references to a wrong thread
      void spawnTasklet(const string& taskletPath);

      bool alive();         // tell application if tasklet is still alive      

      void start(); // starts tasklet thread
      void stop();  // tells tasklet thread to stop and shutdown

      void run();   // the tasklet run loop; platform specific implementation

      virtual void key(const application::KeyEventPtr& ev);      
    };
  }
}

#endif

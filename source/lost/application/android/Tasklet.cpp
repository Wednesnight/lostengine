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

#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/application/WindowEvent.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/Context.h"
#include "lost/common/Logger.h"
#include "lost/resource/Loader.h"
#include "lost/resource/AndroidAssetRepository.h"

#include <sstream>
#include <stdexcept>

extern "C" {
  #include <jni.h>
  #include <android_native_app_glue.h>
  
  extern struct android_app* getAndroidApp();
}

using namespace std;

namespace lost
{
  namespace application
  {

    struct TaskletReferences
    {
      Tasklet* tasklet;
      lost::shared_ptr<TaskletThread> thread;
    };
    
    static void handleAppCmd(struct android_app* app, int32_t cmd)
    {
      TaskletReferences* refs = (TaskletReferences*)app->userData;

      switch (cmd) {
        case APP_CMD_SAVE_STATE:
          break;
        case APP_CMD_INIT_WINDOW:
          refs->thread.reset(new TaskletThread(refs->tasklet));
          refs->thread->start();
          break;
        case APP_CMD_TERM_WINDOW:
          refs->tasklet->eventDispatcher->queueEvent(WindowEvent::create(WindowEvent::CLOSE(), refs->tasklet->window));
          break;
        case APP_CMD_GAINED_FOCUS:
          break;
        case APP_CMD_LOST_FOCUS:
          break;
      }
    }

    struct Tasklet::TaskletHiddenMembers
    {
      struct android_app* app;
      TaskletReferences refs;
    };

    void Tasklet::start()
    {
      hiddenMembers.reset(new TaskletHiddenMembers);

      // setup references & handlers
      hiddenMembers->app = getAndroidApp();
      hiddenMembers->app->userData = &hiddenMembers->refs;
      hiddenMembers->app->onAppCmd = handleAppCmd;
      hiddenMembers->refs.tasklet = this;
    }

    void Tasklet::run()
    {
      isAlive = true;
      bool hasError = false;
      string errorMsg = "";
      
      try {
        // initialize window, gl
        init();
        
        // make sure that our GL context is the current context
        if(window != NULL) {
          window->context->makeCurrent();
        }
        
        startup();
        if (running) {
          
          double framerate = config.framerate;
          double offset = clock.getTime();
          
          while (hiddenMembers->refs.thread->get_state() == fhtagn::threads::tasklet::RUNNING && running) {
            processEvents();
            update(framerate);
            render();
            if(waitForEvents) { eventDispatcher->waitForEvents(); }
            
            framerate = clock.getElapsedAndUpdateOffset(offset);
          }
          
          shutdown();
        }
      }
      catch(std::exception& ex) {
        errorMsg = ex.what();
        hasError = true;
      }
      catch (...) {
        errorMsg = "<catch all>";
        hasError = true;
      }
      isAlive = false;
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));

      if (hasError) {
        
        ostringstream os;
        os << "Tasklet '"<<name<<"' terminated with error: " <<errorMsg;
        throw runtime_error(os.str());
      }
    }

    void Tasklet::stop()
    {
      if (isAlive) {
        hiddenMembers->refs.thread->stop();
        // wakeup
        if (waitForEvents) {
          eventDispatcher->wakeup();
        }
        hiddenMembers->refs.thread->wait();
      }
      else
      {
        // make sure that our GL context is the current context
        if(window != NULL) {
          window->context->makeCurrent();
        }
        cleanup();
        if (running) {
          running = false;
          dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
        }
      }
    }

  }
}


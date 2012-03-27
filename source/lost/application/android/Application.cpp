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

#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Tasklet.h"
#include "lost/application/SpawnTaskletEvent.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"

#include <iostream>
#include <stdexcept>

extern "C" {
  #include <android_native_app_glue.h>
  
  extern int main(int argn, char** args);
  
  static struct android_app* _android_app = NULL;
  
  void setAndroidApp(struct android_app* app) {
    _android_app = app;
  }
  
  struct android_app* getAndroidApp() {
    return _android_app;
  }
  
  void android_main(struct android_app* state) {
    // Make sure glue isn't stripped.
    app_dummy();
    
    setAndroidApp(state);
    main(0, NULL);
  }
}

using namespace std;

namespace lost
{
  namespace application
  {

    struct Application::ApplicationHiddenMembers
    {
      struct android_app* app;
    };

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;
      hiddenMembers->app = getAndroidApp();
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");
      delete hiddenMembers;
    }

    void Application::run(int argc, char *argv[])
    {
      DOUT("Application::run()");

      ApplicationEventPtr event = ApplicationEvent::create(ApplicationEvent::RUN());
      eventDispatcher->dispatchEvent(event);

      struct android_app* app = getAndroidApp();

      int ident;
      int events;
      struct android_poll_source* source;
      while (running) {
        
        while ((ident = ALooper_pollAll(-1, NULL, &events, (void**)&source)) >= 0) {
          
          // Process this event.
          if (source != NULL) {
            source->process(app, source);
          }
          
          // Check if we are exiting.
          if (app->destroyRequested != 0) {
            quit();
          }

          eventDispatcher->processEvents();

          if (!running) {
            break;
          }
        }
          
      }
    }

    void Application::shutdown()
    {
      ALooper_wake(hiddenMembers->app->looper);
    }

    void Application::showMouse(bool visible)
    {
    }

    void Application::processEvents(const ProcessEventPtr& event)
    {
      ALooper_wake(hiddenMembers->app->looper);
    }
  }
}

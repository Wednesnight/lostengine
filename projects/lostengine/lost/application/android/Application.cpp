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

    void Application::taskletSpawn(const SpawnTaskletEventPtr& event)
    {
      addTasklet(new Tasklet(event->loader));
    }

  }
}

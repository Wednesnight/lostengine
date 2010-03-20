#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H


#include <list>
#include <map>
#include <string>
#include "lost/resource/Loader.h"
#include "lost/event/forward.h"
#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    struct Application 
    {
      /**
       * forward declaration for platform specific stuff
       */
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      lost::resource::LoaderPtr loader;

      /**
       * list of tasklets
       * have a look at lost/application/Tasklet.h for details
       */
      std::list<Tasklet*> tasklets;

      bool running;

      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      /**
       * don't use ctors directly! leave them private since we need to be held by a lost::shared_ptr,
       */
      Application(resource::LoaderPtr inLoader = resource::LoaderPtr());
      Application(Tasklet* tasklet, resource::LoaderPtr inLoader = resource::LoaderPtr());

      /**
       * ctor helper
       */
      void initApplication(resource::LoaderPtr inLoader);

      /**
       * Removes the specified tasklet.
       */
      void removeTasklet(Tasklet * tasklet);


      /**
       * listener for ApplicationEvent.RUN()
       * handled in generic code
       * raised from platform specific code
       */
      void startup(ApplicationEventPtr& event);

      /**
       * listener for ApplicationEvent.QUIT()
       */
      void quitHandler(ApplicationEventPtr& event);

      /**
       * handled in platform specific code
       * raised from generic code
       * should be executed on main thread!
       */
      void shutdown();

      void taskletSpawn(const SpawnTaskletEventPtr& event);   // Tasklet spawn handler
      void taskletTerminate(const TaskletEventPtr& event);    // Tasklet terminate handler
      void taskletDone(const TaskletEventPtr& event);         // Tasklet done handler
      void queueEvent(const QueueEventPtr& event);            // listener for QueueEvent.QUEUE()
      void processEvents(const ProcessEventPtr& event);       // should be executed on main thread!

    public:
      lost::event::EventDispatcherPtr eventDispatcher;

      /**
       * static ctor helpers, make sure that we're held by a lost::shared_ptr
       */
      static ApplicationPtr create();
      static ApplicationPtr create(Tasklet* tasklet);

      ~Application();

      void addTasklet(Tasklet* tasklet);  // adds the specified tasklet, calls it's init() and start() methods
      void run(int argc, char *argv[]);   // starts all tasklets and queues a run event
      void quit();                        // quits the app
      void showMouse(bool visible);       // call this to show/hide the OS mouse
    };

  }
}

#endif

#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H


#include <list>
#include <map>
#include <string>
#include "lost/event/forward.h"
#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    struct Application 
    {
    private:
      /**
       * forward declaration for platform specific stuff
       */
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      bool running;

      /**
       * hidden constructor
       */
      Application(Tasklet* tasklet);
      
      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      /**
       * Removes the specified tasklet.
       */
      void removeTasklet(Tasklet * tasklet);


      /**
       * listener for ApplicationEvent.RUN()
       * handled in generic code
       * raised from platform specific code
       */
      void startup(const ApplicationEventPtr& event);

      /**
       * listener for ApplicationEvent.QUIT()
       */
      void quitHandler(const ApplicationEventPtr& event);

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
      /**
       * list of tasklets
       * see: lost/application/Tasklet.h
       */
      std::list<Tasklet*> tasklets;
      
      /**
       * see: lost/event/EventDispatcher.h
       */
      lost::event::EventDispatcherPtr eventDispatcher;

      /**
       * returns Application singleton
       */
      static ApplicationPtr getInstance(Tasklet* tasklet = NULL);

      ~Application();

      void addTasklet(Tasklet* tasklet);  // adds the specified tasklet, calls it's init() and start() methods
      void run(int argc, char *argv[]);   // starts all tasklets and queues a run event
      void quit();                        // quits the app
      void showMouse(bool visible);       // call this to show/hide the OS mouse
    };

  }
}

#endif

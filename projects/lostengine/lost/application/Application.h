#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H


#include <list>
#include <map>
#include <string>
#include "lost/resource/Loader.h"
#include "lost/event/Event.h"

namespace lost
{
  namespace event
  {
    struct EventDispatcher;
    typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr;
  }

  namespace application
  {
    struct Tasklet;
    
    struct TaskletEvent;
    typedef lost::shared_ptr<TaskletEvent> TaskletEventPtr;

    struct Application;
    typedef lost::shared_ptr<Application> ApplicationPtr;

    struct ApplicationEvent;
    typedef lost::shared_ptr<ApplicationEvent> ApplicationEventPtr;

    struct SpawnTaskletEvent;
    typedef lost::shared_ptr<SpawnTaskletEvent> SpawnTaskletEventPtr;
    
    struct QueueEvent;
    typedef lost::shared_ptr<QueueEvent> QueueEventPtr;
    
    struct ProcessEvent;
    typedef lost::shared_ptr<ProcessEvent> ProcessEventPtr;
    
    int runTasklet(Tasklet* t);


    struct Application : public lost::enable_shared_from_this<Application>
    {
    private:
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

      /**
       * running flag
       */
      bool running;

      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      /**
       * don't use ctors directly! leave them private since we need to be held by a lost::shared_ptr,
       * otherwise shared_from_this() will fail!
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
    public:
      lost::event::EventDispatcherPtr eventDispatcher;

      /**
       * static ctor helpers, make sure that we're held by a lost::shared_ptr
       */
      static lost::shared_ptr<Application> create();
      static lost::shared_ptr<Application> create(Tasklet* tasklet);

      ~Application();

      /**
       * adds the specified tasklet
       * if the application is already running you have to take care of starting the tasklet yourself
       */
      void addTasklet(Tasklet* tasklet);

      /**
       * starts all tasklets and queues a run event
       */
      void run();

      /**
       * quits the app
       */
      void quit();

      /**
       * call this to show/hide the OS mouse
       */
      void showMouse(bool visible);      

      /**
       * Tasklet spawn handler
       **/
      void taskletSpawn(const SpawnTaskletEventPtr& event);

      /**
       * Tasklet done handler
       **/
      void taskletDone(const TaskletEventPtr& event);

      /**
       * listener for QueueEvent.QUEUE()
       */
      void queueEvent(const QueueEventPtr& event);

      /**
       * should be executed on main thread!
       */
      void processEvents(const ProcessEventPtr& event);
    };

  }
}

#endif

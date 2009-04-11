#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H


#include <list>
#include <map>
#include <string>
#include <boost/enable_shared_from_this.hpp>
#include "lost/resource/Loader.h"
#include "lost/event/Event.h"

namespace lost
{
  namespace event
  {
    struct EventDispatcher;
    typedef boost::shared_ptr<EventDispatcher> EventDispatcherPtr;
  }

  namespace application
  {
    struct Tasklet;
    typedef boost::shared_ptr<Tasklet> TaskletPtr;
    
    struct Application;
    typedef boost::shared_ptr<Application> ApplicationPtr;

    struct ApplicationEvent;
    typedef boost::shared_ptr<ApplicationEvent> ApplicationEventPtr;

    struct Application : public boost::enable_shared_from_this<Application>
    {
    private:
      /**
       * forward declaration for platform specific stuff
       */
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      /**
       * list of tasklets
       * have a look at lost/application/Tasklet.h for details
       */
      std::list<TaskletPtr> tasklets;

      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      /**
       * don't use ctors directly! leave them private since we need to be held by a boost::shared_ptr,
       * otherwise shared_from_this() will fail!
       */
      Application(resource::LoaderPtr inLoader = resource::LoaderPtr());
      Application(int argn, char** args, resource::LoaderPtr inLoader = resource::LoaderPtr());
      Application(const TaskletPtr& tasklet, resource::LoaderPtr inLoader = resource::LoaderPtr());
      Application(const std::string& inScript, resource::LoaderPtr inLoader = resource::LoaderPtr());

      /**
       * ctor helper
       */
      void initApplication(resource::LoaderPtr inLoader);

      /**
       * listener for ApplicationEvent.RUN()
       * handled in generic code
       * raised from platform specific code
       */
      void startup(ApplicationEventPtr& event);

      /**
       * listener for ApplicationEvent.QUIT(), implemented in platform specific parts
       * handled in platform specific code
       * raised from generic code
       */
      void shutdown(ApplicationEventPtr& event);
    public:
      lost::resource::LoaderPtr loader;
      lost::event::EventDispatcherPtr eventDispatcher;
      
      /**
       * static ctor helpers, make sure that we're held by a boost::shared_ptr
       */
      static boost::shared_ptr<Application> create();
      static boost::shared_ptr<Application> create(int argn, char** args);
      static boost::shared_ptr<Application> create(const TaskletPtr& tasklet);
      static boost::shared_ptr<Application> create(const std::string& inScript);

      ~Application();

      /**
       * adds the specified tasklet
       * if the application is already running you have to take care of starting the tasklet yourself
       */
      void addTasklet(const TaskletPtr& tasklet);

      /**
       * dispatches application events into all active tasklets
       */
      void dispatchApplicationEvent(const lost::event::Type& which);
      
      /**
       * starts all tasklets and queues a run event
       */
      void run();

      /**
       * queues a quit event
       */
      void quit();

      /**
       * terminates all tasklets, called from platform specific code when shutdown is complete
       */
      void terminate();

      /**
       * call this to show/hide the OS mouse
       */
      void showMouse(bool visible);      
    };

  }
}

#endif

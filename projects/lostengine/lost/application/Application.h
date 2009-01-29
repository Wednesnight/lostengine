#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H

// FIXME: needed for NOMINMAX definition on windows
#include "lost/platform/Platform.h"

#include <list>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem.hpp>

#include "lost/application/RunLoopThread.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/ApplicationEvent.h"

namespace lost
{
  namespace application
  {

    struct Application : public boost::enable_shared_from_this<Application>, public lost::event::EventDispatcher
    {
    private:
      /**
       * forward declaration for platform specific stuff
       */
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      /**
       * the RunLoopThread is as wrapper for boost::thread plus application specific runloop implementation
       * have a look at lost/application/RunLoopThread.h for details
       */
      boost::shared_ptr<RunLoopThread> runLoopThread;

      /**
       * applications windows are created using a unique id (named windows), this list maps uniqueId -> window
       */
      std::map<std::string, boost::shared_ptr<Window> > windows;

      /**
       * hidden ctor/dtor utility methods for platform specific stuff
       */
      void initialize();
      void finalize();

      /**
       * hidden utility methods called from run() and quit() implementing platform specific stuff [again! :)]
       */
      void doRun();
      void doQuit();

      /**
       * don't use ctors directly! leave them private since we need to be held by a boost::shared_ptr,
       * otherwise shared_from_this() will fail!
       */
      Application();
      Application(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);
      Application(const boost::filesystem::path& inScript);
    public:
      /**
       * static ctor helpers, make sure that we're held by a boost::shared_ptr
       */
      // TODO: add ctor taking custom RunLoopThread
      static boost::shared_ptr<Application> create();
      static boost::shared_ptr<Application> create(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);
      static boost::shared_ptr<Application> create(const boost::filesystem::path& inScript);

      ~Application();

      /**
       * set the application's runloop, has to be set before run() is invoked!
       * will be started in a seperate thread, so please use the synchronized EventDispatcher methods (queueEvent, processEvents)
       */
      void setRunLoop(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);

      /**
       * returns a generic interface to a new Window object, will fail if a window with that uniqueId already exists
       */
      boost::shared_ptr<Window> createWindow(const std::string& uniqueId, const WindowParams& params);

      /**
       * starts the runloop and event handling mechanisms
       */
      void run();

      /**
       * queues a quit event and calls terminate
       */
      void quit();

      /**
       * do NOT call this directly! make sure that a quit event is queued otherwise the synchronisation with the RunLoopThread will lead to a deadlock
       */
      // FIXME: make this private (currently needed by Cocoa NSApp delegate)
      void terminate();
    };

  }
}

#endif

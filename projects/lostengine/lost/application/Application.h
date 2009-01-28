#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H

// FIXME: needed for NOMINMAX definition on windows
#include "lost/platform/Platform.h"

#include <list>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/filesystem.hpp>

#include "lost/application/RunLoop.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"

namespace lost
{
  namespace application
  {

    struct Application : public lost::event::EventDispatcher, public boost::enable_shared_from_this<Application>
    {
    private:
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      boost::shared_ptr<boost::thread> runLoopThread;

      boost::mutex queueMutex;
      boost::shared_ptr<std::list<boost::shared_ptr<lost::event::Event> > > eventQueue;

      std::map<std::string, boost::shared_ptr<Window> > windows;

      void initialize();
      void finalize();
    public:
      boost::shared_ptr<RunLoop> runLoop;

      Application();
      Application(const boost::function<void (void)>& inRunLoopFunction);
      Application(const boost::shared_ptr<RunLoop>& inRunLoop);
      Application(const boost::filesystem::path& inRunLoopScript);

      ~Application();
      
      boost::shared_ptr<Window> createWindow(const std::string& uniqueId, const WindowParams& params);
      void run();
      void quit();

      void queueEvent(const boost::shared_ptr<lost::event::Event>& event); // call this to queue the given event. will be dispatched when processEvents() is called
      void processEvents(const double& timeoutInSeconds = 0); // call this to signal queued events
    };

  }
}

#endif

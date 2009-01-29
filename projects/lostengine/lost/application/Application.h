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
      struct ApplicationHiddenMembers;
      ApplicationHiddenMembers* hiddenMembers;

      boost::shared_ptr<RunLoopThread> runLoopThread;

      std::map<std::string, boost::shared_ptr<Window> > windows;

      void initialize();
      void finalize();

      Application();
      Application(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);
      Application(const boost::filesystem::path& inScript);
    public:
      static boost::shared_ptr<Application> create();
      static boost::shared_ptr<Application> create(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);
      static boost::shared_ptr<Application> create(const boost::filesystem::path& inScript);

      ~Application();

      void setRunLoop(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);

      boost::shared_ptr<Window> createWindow(const std::string& uniqueId, const WindowParams& params);
      void run();
      void quit();
      void terminate();
    };

  }
}

#endif

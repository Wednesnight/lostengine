#ifndef LOST_APPLICATION_RUNLOOP_H
#define LOST_APPLICATION_RUNLOOP_H

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include <boost/signal.hpp>
#include <boost/thread.hpp>

#include "lost/event/Event.h"
#include "lost/common/Logger.h"
#include "lost/lua/State.h"
#include "lost/lua/ModuleLoader.h"
#include "lost/lua/BindAll.h"

namespace lost
{
  namespace application
  {

    struct Application;
    struct RunLoopThread
    {
    protected:
      bool running;
      boost::shared_ptr<Application> application;
      boost::shared_ptr<boost::thread> thread;
      boost::function<void (const boost::shared_ptr<Application>& sender)> runLoop;

      virtual void loop();
      void quit(boost::shared_ptr<event::Event> event);
    public:
      RunLoopThread();
      RunLoopThread(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);

      void setRunLoop(const boost::function<void (const boost::shared_ptr<Application>& sender)>& inRunLoop);

      virtual void initialize(const boost::shared_ptr<Application>& inApplication);
      virtual void run(const boost::shared_ptr<Application>& inApplication);
      void join();
      bool waitForEvents; // if true, only runs the loop once a low level event arrives
    };

    struct RunLoopThreadLua : public RunLoopThread
    {
    protected:
      boost::filesystem::path             filename;
      boost::shared_ptr<lost::lua::State> interpreter;
    public:
      RunLoopThreadLua(const boost::filesystem::path& inFilename);
      virtual void initialize(const boost::shared_ptr<Application>& inApplication);
    };

  }
}

#endif

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
    typedef boost::shared_ptr<Application> ApplicationPtr;
    typedef boost::function<void (const ApplicationPtr& sender)> RunLoopFunction;

    struct RunLoopThread
    {
    protected:
      bool running;
      ApplicationPtr application;
      boost::shared_ptr<boost::thread> thread;
      boost::function<void (const ApplicationPtr& sender)> runLoop;

      virtual void loop();
      void quit(event::EventPtr event);
    public:
      RunLoopThread();
      RunLoopThread(const RunLoopFunction& inRunLoop);

      void setRunLoop(const RunLoopFunction& inRunLoop);

      virtual void initialize(const ApplicationPtr& inApplication);
      virtual void run(const ApplicationPtr& inApplication);
      void join();
      bool waitForEvents; // if true, only runs the loop once a low level event arrives
    };

    struct RunLoopThreadLua : public RunLoopThread
    {
    protected:
      boost::filesystem::path filename;
      lost::lua::StatePtr     interpreter;
    public:
      RunLoopThreadLua(const boost::filesystem::path& inFilename);
      virtual void initialize(const ApplicationPtr& inApplication);
    };

  }
}

#endif

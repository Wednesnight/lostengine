#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H

#include "lost/forward/boost"
#include "lost/common/DisplayAttributes.h"
#include "lost/event/EventDispatcher.h"
#include "lost/lua/State.h"
#include "lost/resource/Loader.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/Config.h"
#include "lost/gl/Context.h"
#include "lost/application/MainLoop.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

struct ApplicationAdapter;

namespace lost
{
  namespace application
  {

    struct Application : public event::EventDispatcher
    {
    public:
      Application();
      Application(boost::function<void (void)> mainLoopFunc);
      Application(boost::shared_ptr<MainLoop> inMainLoop);
      virtual ~Application();

      void run();
      void swapBuffers();
      void quit();

      boost::shared_ptr<common::DisplayAttributes>    displayAttributes;
      boost::shared_ptr<resource::Loader>             loader;
      boost::shared_ptr<lua::State>                   interpreter;
      Config::SharedPtr                               config;
      boost::shared_ptr<lost::gl::Context>            context;
      boost::shared_ptr<lost::event::EventDispatcher> screen;

      void handleResize(boost::shared_ptr<lost::application::ResizeEvent> ev);      

      virtual void dispatchEvent(lost::event::EventPtr event);

      boost::mutex queueMutex;
      boost::shared_ptr<std::list<boost::shared_ptr<lost::event::Event> > > eventQueue;
      void queueEvent(const boost::shared_ptr<lost::event::Event>& event); // call this to queue the given event. will be dispatched when processEvents() is called
      void processEvents(const double& timeoutInSeconds = 0); // call this to signal queued events
      
      boost::shared_ptr<MainLoop> mainLoop;
      boost::shared_ptr<boost::thread> mainLoopThread;

    private:
      void init();
      boost::shared_ptr<ApplicationAdapter> adapter;
      
    };

    extern Application* appInstance;

  }
}

#endif

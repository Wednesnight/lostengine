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

struct ApplicationAdapter;

namespace lost
{
  namespace application
  {

    struct Application : public event::EventDispatcher
    {
    public:
      Application();
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
      
    private:
      boost::shared_ptr<ApplicationAdapter> adapter;
      
    };

    extern Application* appInstance;

  }
}

#endif

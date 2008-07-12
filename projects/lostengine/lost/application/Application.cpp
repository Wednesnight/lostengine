#include "lost/application/Application.h"
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>
#include "lost/application/ApplicationEvent.h"

using namespace std;
using namespace boost;

namespace lost
{
namespace application
{
  Application* appInstance = NULL;

  Application::Application()
  : adapter(this)
  {
    // make sure the global application pointer is set to a correct value
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
    }
    appInstance = this;

    // the main loop is not running yet
    running = false;

    // init default resource loader
    // init lua state with resource loader
    // try to load default lua script
    
    // broadcast preinit event, this is the latest point for client code to setup the configuration
    shared_ptr<ApplicationEvent> appEvent(new ApplicationEvent(ApplicationEvent::PREINIT()));dispatchEvent(appEvent);
    
    // try to extract default display attributes from lua state
    
    // init display/adapter so all other code is safe from here on because an OpenGL context now exists
    adapter.init(displayAttributes);
    
    // broadcast init event so dependant code knows its safe to init resources now
    appEvent->type = ApplicationEvent::INIT();dispatchEvent(appEvent);

    // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
    addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, this, _1)));

    // broadcast postinit event to signal that the application initialisation has finished
    appEvent->type = ApplicationEvent::POSTINIT();dispatchEvent(appEvent);
  }
  
  Application::~Application() { adapter.terminate(); }

  void Application::handleResize(boost::shared_ptr<ResizeEvent> ev)
  {
    displayAttributes.width = ev->width;
    displayAttributes.height = ev->height;
  }

  void Application::run()
  {
    appEvent->type = ApplicationEvent::RUN();dispatchEvent(appEvent);
    running = true;
    double lastTime = adapter.getTime();
    while(running)
    {
      double now = adapter.getTime();
      double delta = now - lastTime;
      lastTime = now;
      timerManager.updateTimers(delta);
      adapter.pollEvents();
      adapter.sleep(.001); // stupid fucking hack because glfw doesn't support OS timers and we need to measure manually.
      if(!adapter.displayOpen())
      {
        running = false;
      }
    }
    appEvent->type = ApplicationEvent::QUIT();dispatchEvent(appEvent);
  }

  void Application::quit() { running=false; }
}
}

#include "lost/application/Application.h"
#include <GL/glfw.h>
#include <iostream>
#include <algorithm>
#include "lost/application/Timer.h"
#include <boost/bind.hpp>

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
    adapter.init(displayAttributes);
    if(appInstance != NULL)
    {
      throw std::runtime_error("appInstance wasn't NULL, did you instance more than one app?");
    }
    appInstance = this;
    running = false;

    // connect resize callback to ourselves so we can keep track of the window size in the displayAttributes
    addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), event::receive<ResizeEvent>(bind(&Application::handleResize, this, _1)));
  }
  
  Application::~Application() { glfwTerminate(); }

  void Application::handleResize(boost::shared_ptr<ResizeEvent> ev)
  {
    displayAttributes.width = ev->width;
    displayAttributes.height = ev->height;
  }

  void Application::run()
  {
    running = true;
    double lastTime = glfwGetTime();
    while(running)
    {
      double now = glfwGetTime();
      double delta = now - lastTime;
      lastTime = now;
      updateTimers(delta);
      glfwPollEvents();
      glfwSleep(.001); // stupid fucking hack because glfw doesn't support OS timers and we need to measure manually.
      if(!glfwGetWindowParam(GLFW_OPENED))
      {
        running = false;
      }
    }
  }

  void Application::updateTimers(double deltaSec)
  {
    for(list<Timer*>::iterator i = timers.begin(); i!=timers.end(); ++i)
    {
      (*i)->update(deltaSec);
    }
  }

  void Application::quit() { running=false; }

  void Application::addTimer(Timer* timer)
  {
    timers.push_back(timer);
  }

  void Application::removeTimer(Timer* timer)
  {
    list<Timer*>::iterator pos = find(timers.begin(), timers.end(), timer);
    if(pos != timers.end())
    {
      timers.erase(pos);
    }
    else
    {
      WOUT("tried to remove timer "<<timer->name() << " which wasn't previously added");
    }
  };
}
}

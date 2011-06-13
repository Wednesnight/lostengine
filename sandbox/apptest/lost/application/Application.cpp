#include "lost/application/Application.h"
#include "lost/event/EventDispatcher.h"
namespace lost
{
namespace application
{

ApplicationPtr Application::getInstance()
{
  static ApplicationPtr instance;
  if (!instance) {
    instance.reset(new Application());
  }
  return instance;
}

Application::Application()
{
  running = false;
  eventDispatcher.reset(new event::EventDispatcher);
}

Application::~Application()
{
}

void Application::addTasklet(Tasklet* tasklet)
{
}

void Application::quit()
{
}

}
}
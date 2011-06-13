#include "lost/application/Application.h"

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
}

Application::~Application()
{
}

void Application::run(int argc, char *argv[])
{
}


void Application::startTasklet(Tasklet* tasklet)
{
}

}
}
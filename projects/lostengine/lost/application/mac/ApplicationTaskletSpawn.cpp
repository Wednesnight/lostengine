#include "lost/application/Application.h"
#include "lost/application/MultiThreadedTasklet.h"
#include "lost/application/SpawnTaskletEvent.h"

// needs to be in a separate file that's not .mm because objective-c++ can't handle the Tasklet/luabind code
namespace lost
{
  namespace application
  {
    // is platform specific because we need to determine the default type for each platform which might eb compile time dependent
    void Application::taskletSpawn(const SpawnTaskletEventPtr& event)
    {
      addTasklet(new MultiThreadedTasklet(event->loader));
    }
  }
}

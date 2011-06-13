#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H

#include "lost/application/forward.h"
#include "EASTL/list.h"
#include "lost/event/forward.h"

namespace lost
{
namespace application
{

struct Application
{
  struct ApplicationHiddenMembers;
  ApplicationHiddenMembers* hiddenMembers;
  
  Application();
  virtual ~Application();
  
  static ApplicationPtr getInstance();
  
  void run(int argc, char *argv[]);   // starts all tasklets and queues a run event
  void initialize();
  void finalize();
  void shutdown();
  void quit();
  void addTasklet(Tasklet* tasklet);
  void showMouse(bool visible);
  void processEvents(const ProcessEventPtr& event);

  void taskletSpawn(const SpawnTaskletEventPtr& event);
  
  bool running;
  eastl::list<Tasklet*> tasklets;
  event::EventDispatcherPtr eventDispatcher;
};

}
}

#endif
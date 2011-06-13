#ifndef LOST_APPLICATION_APPLICATION_H
#define LOST_APPLICATION_APPLICATION_H

#include "lost/application/forward.h"
#include "EASTL/list.h"

namespace lost
{
namespace application
{

struct Application
{
  Application();
  virtual ~Application();
  
  static ApplicationPtr getInstance();
  
  void run(int argc, char *argv[]);   // starts all tasklets and queues a run event

  
  void startTasklet(Tasklet* tasklet);
  
  bool running;
  eastl::list<Tasklet*> tasklets;
};

}
}

#endif
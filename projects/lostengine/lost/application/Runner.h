#ifndef LOST_APPLICATION_RUNNER_H
#define LOST_APPLICATION_RUNNER_H

namespace lost
{
namespace application
{

  struct Tasklet;

  int runDefaultTasklet();
  int runTasklet(Tasklet* t);
    
}
}

#endif
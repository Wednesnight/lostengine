/**
 *  TaskletThread is a lost::application::Tasklet specific wrapper for fhtagn::threads::tasklet
 *
 *  Use this wrapper for multi-threaded, platform-specific Tasklet implementations
 *  (see mac/Tasklet.mm, windows/Tasklet.cpp)
 */

#ifndef LOST_APPLICATION_TASKLETTHREAD_H
#define LOST_APPLICATION_TASKLETTHREAD_H

#include "fhtagn/threads/tasklet.h"
#include "lost/application/Tasklet.h"

namespace lost
{
  namespace application
  {

    struct TaskletThread : public fhtagn::threads::tasklet
    {
    protected:
      Tasklet* tasklet;
      void run(fhtagn::threads::tasklet& tasklet);
      void error(fhtagn::threads::tasklet& tasklet, std::exception const& exception);
    public:
      TaskletThread(Tasklet* inTasklet);
    };

  }
}

#endif

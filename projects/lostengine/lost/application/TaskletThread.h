/**
 *  TaskletThread is a lost::application::Tasklet specific wrapper for fhtagn::threads::tasklet
 *
 *  Use this wrapper for multi-threaded, platform-specific Tasklet implementations
 *  (see mac/Tasklet.mm, windows/Tasklet.cpp)
 */

#ifndef LOST_APPLICATION_TASKLETTHREAD_H
#define LOST_APPLICATION_TASKLETTHREAD_H

//#include "fhtagn/threads/tasklet.h"
#include "lost/application/Tasklet.h"
#include "tinythread.h"

namespace lost
{
  namespace application
  {

    struct TaskletThread 
    {
      enum state
      {
          STANDING_BY = 0,   /* Tasklet is not currently running the bound
                                function, so start() can safely be invoked. */
          RUNNING     = 1,   /* Tasklet is currently running the bound function.
                                You can't start() it, but you can stop() it. */
          SLEEPING    = 2,   /* The tasklet's bound function has invoked sleep(),
                                but not yet returned. */
          STOPPED     = 3,   /* The stop() function has been called, but the bound
                                function has not yet finished execution. */
          ABORTED     = 4,   /* Running the bound function resulted in an
                                exception, and has stopped. */
          FINISHED    = 5    /* The bound function has finished executing,
                                possibly because stop() was called. The tasklet is
                                ready for resetting. */
      };
    
      Tasklet* tasklet;
      TaskletThread(Tasklet* inTasklet);
      tthread::thread* _thread;
      tthread::mutex _mutex;
      tthread::condition_variable _finish;
      state _state;
      state get_state();
      bool alive();
      bool start();
      bool stop();
      bool wait();
    };

  }
}

#endif

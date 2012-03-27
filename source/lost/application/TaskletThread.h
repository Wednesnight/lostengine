/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 *  TaskletThread is a lost::application::Tasklet specific wrapper for fhtagn::threads::tasklet
 *
 *  Use this wrapper for multi-threaded, platform-specific Tasklet implementations
 *  (see mac/Tasklet.mm, windows/Tasklet.cpp)
 */

#ifndef LOST_APPLICATION_TASKLETTHREAD_H
#define LOST_APPLICATION_TASKLETTHREAD_H

#include "lost/application/forward.h"
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

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

#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/common/Logger.h"

#pragma warning(disable:4355) //warning C4355: 'this' : used in base member initializer list

#define IS_ALIVE \
    (RUNNING == _state || SLEEPING == _state || STOPPED == _state)

using namespace tthread;

namespace lost
{
  namespace application
  {
      void leTaskletThreadRun(void* tt)
      {
        TaskletThread* taskletThread = (TaskletThread*)tt;
        try {
          taskletThread->tasklet->run();
        }
        catch (std::exception& ex) {
          lock_guard<mutex> lock(taskletThread->_mutex);
          taskletThread->_state = TaskletThread::ABORTED;
          EOUT(ex.what());
        }
        catch(...)
        {
          lock_guard<mutex> lock(taskletThread->_mutex);
          taskletThread->_state = TaskletThread::ABORTED;
          EOUT("tasklet aborted with unknown error");
        }
        
        {
          lock_guard<mutex> lock(taskletThread->_mutex);
          if ((taskletThread->_state == TaskletThread::RUNNING)
              ||
              (taskletThread->_state == TaskletThread::SLEEPING)
              ||
              (taskletThread->_state == TaskletThread::STOPPED)
             )
          {
            taskletThread->_state = TaskletThread::FINISHED;
          }
        }
        taskletThread->_finish.notify_all();
      }
      

      TaskletThread::TaskletThread(Tasklet* inTasklet)
       : tasklet(inTasklet)
      {
        _state = STANDING_BY;
        _thread = NULL;
      }

      bool TaskletThread::start()
      {
        lock_guard<mutex> lock(_mutex);
        if(_thread)
        {
          return false;
        }        
        _state = RUNNING;
        _thread = new tthread::thread(leTaskletThreadRun, this);
        return true;
      }

      TaskletThread::state TaskletThread::get_state()
      {
        lock_guard<mutex> lock(_mutex);        
        return _state;
      }

      bool TaskletThread::stop()
      {
        lock_guard<mutex> lock(_mutex);        
        if(!_thread) return false;
        if(IS_ALIVE)
        {
          _state = STOPPED;
        }
        _finish.notify_all();
        return true;
      }

      bool TaskletThread::wait()
      {
        lock_guard<mutex> lock(_mutex);
        if(!_thread) return false;
        while(IS_ALIVE)
        {
          _finish.wait(_mutex);
        }
        thread* join_thread = _thread;
        _thread = NULL;
        _mutex.unlock();
        join_thread->join();
        delete join_thread;
        return true;
      }

/*      void TaskletThread::run(fhtagn::threads::tasklet& t)
      {
        tasklet->run();
      }

      void TaskletThread::error(fhtagn::threads::tasklet& t, std::exception const& e)
      {
        EOUT(e.what());
      }*/

  }
}

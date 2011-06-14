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
//      : fhtagn::threads::tasklet(bind(&TaskletThread::run, this, _1)),
       : tasklet(inTasklet)
      {
//        add_error_handler(boost::bind(&TaskletThread::error, this, _1, _2));
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

#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace application
  {

      TaskletThread::TaskletThread(Tasklet* inTasklet)
      : fhtagn::threads::tasklet(bind(&TaskletThread::run, this, _1)),
        tasklet(inTasklet)
      {
      }

      void TaskletThread::run(fhtagn::threads::tasklet& t)
      {
        tasklet->run();
      }

      void TaskletThread::error(fhtagn::threads::tasklet& t, std::exception const& e)
      {
        EOUT(e.what());
        tasklet->dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), tasklet)));
      }

  }
}

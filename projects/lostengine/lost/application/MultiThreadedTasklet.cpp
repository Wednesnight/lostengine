#include "lost/application/MultiThreadedTasklet.h"
#include "lost/common/Logger.h"
#include "lost/event/EventDispatcher.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/mac/ThreadAutoreleasePoolHack.h"

using namespace std;

namespace lost
{
namespace application
{

using namespace lost::event;

MultiThreadedTasklet::MultiThreadedTasklet()
: tasklet::tasklet(bind(&MultiThreadedTasklet::run, this, _1))
{
  add_error_handler(bind(&MultiThreadedTasklet::error, this, _1, _2));

}

MultiThreadedTasklet::~MultiThreadedTasklet()
{
}

void MultiThreadedTasklet::error(fhtagn::threads::tasklet& tasklet, std::exception const& exception)
{
  EOUT(exception.what());
  dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
}

bool MultiThreadedTasklet::start()
{
  // init must've been called before this by application or some other external code
  return ::fhtagn::threads::tasklet::start();
}

bool MultiThreadedTasklet::stop()
{
  bool result = ::fhtagn::threads::tasklet::stop();
  if (result)
  {
    eventDispatcher->wakeup();
  }
  return result;
}

void MultiThreadedTasklet::run(tasklet& tasklet)
{
  void* pool = threadAutoreleasePoolHack_createPool();

  if (startup())
  {
    while (get_state() == RUNNING && update())
    {

      render();
      if(!waitForEvents)
      {
        processEvents();
      }
      else
      {
        eventDispatcher->waitForEvents();
        processEvents();
      }

      pool = threadAutoreleasePoolHack_drainAndRecreatePool(pool);
    }
    shutdown();
  }
  threadAutoreleasePoolHack_drainPool(pool);        
  dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
}


}
}
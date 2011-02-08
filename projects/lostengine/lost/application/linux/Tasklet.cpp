#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/Context.h"
#include "lost/common/Logger.h"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace lost
{
  namespace application
  {

    struct Tasklet::TaskletHiddenMembers
    {
      lost::shared_ptr<TaskletThread> thread;
    };

    void Tasklet::start()
    {
      hiddenMembers.reset(new TaskletHiddenMembers);
      hiddenMembers->thread.reset(new TaskletThread(this));
      hiddenMembers->thread->start();
    }

    void Tasklet::run()
    {
      isAlive = true;
      init();
      bool hasError = false;
      std::string errorMsg;

      try
      {
        startup();
        if (running) {
          double framerate = config.framerate;
          double offset = clock.getTime();

          while (hiddenMembers->thread->get_state() == fhtagn::threads::tasklet::RUNNING && running) {
            processEvents();
            update(framerate);
            render();

	    if (waitForEvents) {
	      eventDispatcher->waitForEvents();
	    }

            framerate = clock.getElapsedAndUpdateOffset(offset);
          }

          shutdown();
        }
      }
      catch (std::exception& e)
      {
        errorMsg = e.what();
        hasError = true;
      }

      isAlive = false;
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      cleanup();
      if (hasError)
      {
        ostringstream os;
        os << "Tasklet terminated with error: " << errorMsg;
        throw runtime_error(os.str());
      }
    }

    void Tasklet::stop()
    {
      if (isAlive)
      {
        hiddenMembers->thread->stop();
        // wakeup
        if (waitForEvents)
        {
          eventDispatcher->wakeup();
        }
        hiddenMembers->thread->wait();
      }
    }

  }
}


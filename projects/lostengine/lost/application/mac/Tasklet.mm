#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include <sstream>
#include <stdexcept>
#include "lost/gl/Context.h"
#include <Foundation/NSAutoreleasePool.h>
#include <float.h>


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
      init();
      hiddenMembers.reset(new TaskletHiddenMembers);
      hiddenMembers->thread.reset(new TaskletThread(this));
      hiddenMembers->thread->start();
    }

    void Tasklet::run() {

      isAlive = true;
      bool hasError = false;
      std::string errorMsg = "";

      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

      try {

        // make sure that our GL context is the current context
        if(window != NULL) {
          window->context->makeCurrent();
        }

        startup();
        if (running) {

          double framerate = config.framerate;
          double offset = timer.getTime();

          while (hiddenMembers->thread->get_state() == fhtagn::threads::tasklet::RUNNING && running) {
            processEvents();
            update(framerate);
            render();
            if(waitForEvents) { eventDispatcher->waitForEvents(); }

            framerate = timer.getElapsedAndUpdateOffset(offset);

            [pool drain];
            pool = [[NSAutoreleasePool alloc] init];  
          }

          shutdown();
        }
      }
      catch(std::exception& ex)
      {
        errorMsg = ex.what();
        hasError = true;
      }
      catch (...) {
        errorMsg = "<catch all>";
        hasError = true;
      }
      isAlive = false;
      dispatchApplicationEvent(TaskletEventPtr(new TaskletEvent(TaskletEvent::DONE(), this)));
      [pool drain];
      if (hasError) {

        ostringstream os;
        os << "Tasklet '"<<name<<"' terminated with error: " <<errorMsg;
        throw runtime_error(os.str());
      }
    }

    void Tasklet::stop()
    {
      if (isAlive)
      {
        hiddenMembers->thread->stop();
        // wakeup
        if (waitForEvents) eventDispatcher->wakeup();
        hiddenMembers->thread->wait();
      }
      else
      {
        // make sure that our GL context is the current context
        if(window != NULL)
        {
          window->context->makeCurrent();
        }
        cleanup();
      }
    }

  }
}

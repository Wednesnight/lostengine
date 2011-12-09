#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/Context.h"
#include "lost/platform/Platform.h"

#include <sstream>
#include <stdexcept>

using namespace std;

namespace lost
{
  namespace application
  {

    void Tasklet::run() {
      platform::setThreadName("'"+name+"' (tasklet)");
      isAlive = true;
      bool hasError = false;
      string errorMsg = "";

      try {

        // make sure that our GL context is the current context
        if(window != NULL) {
          window->context->makeCurrent();
        }

        startup();
        if (running) {

          double framerate = config.framerate;
          double offset = clock.getTime();

          while (thread->get_state() == TaskletThread::RUNNING && running) {
            processEvents();
            if (running) {
              update(framerate);
              render();
              if(waitForEvents) { eventDispatcher->waitForEvents(); }

              framerate = clock.getElapsedAndUpdateOffset(offset);
            }
          }

          shutdown();

          // unbind GL context
          if(window != NULL) {
            window->context->clearCurrent();
          }
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
      if (hasError) {

        std::ostringstream os;
        os << "Tasklet '"<<name<<"' terminated with error: " <<errorMsg;
        throw std::runtime_error(os.str());
      }
    }

  }
}

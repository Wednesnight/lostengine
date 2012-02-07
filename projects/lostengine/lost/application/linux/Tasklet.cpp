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
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include "lost/gl/Context.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"

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

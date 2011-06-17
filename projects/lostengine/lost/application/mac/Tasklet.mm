#include "lost/application/Tasklet.h"
#include "lost/application/TaskletThread.h"
#include "lost/application/TaskletEvent.h"
#include "lost/application/Window.h"
#include "lost/event/EventDispatcher.h"
#include <sstream>
#include <stdexcept>
#include "lost/gl/Context.h"
#include <Foundation/NSAutoreleasePool.h>
#include <Foundation/NSArray.h>
#include <float.h>

#import <AppKit/NSPasteboard.h>

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
      platform::setThreadName("'"+name+"' (tasklet)");
      isAlive = true;
      bool hasError = false;
      string errorMsg = "";

      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

      try {

        // make sure that our GL context is the current context
        if(window != NULL) {
          window->context->makeCurrent();
        }

        startup();
        if (running) {

          double framerate = config.framerate;
          double offset = clock.getTime();

          while (hiddenMembers->thread->get_state() == TaskletThread::RUNNING && running) {
            processEvents();
            if (running) {
              update(framerate);
              render();
              if(waitForEvents) { eventDispatcher->waitForEvents(); }

              framerate = clock.getElapsedAndUpdateOffset(offset);
            }

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

        std::ostringstream os;
        os << "Tasklet '"<<name<<"' terminated with error: " <<errorMsg;
        throw std::runtime_error(os.str());
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

    string Tasklet::getClipboardString()
    {
      string str;
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      NSPasteboard* pboard = [NSPasteboard pasteboardWithName: NSGeneralPboard];
      if ([[pboard types] containsObject: NSStringPboardType]) {
        NSString* s = [pboard stringForType: NSStringPboardType];
        if (s != nil) {
          str = string([s cStringUsingEncoding: NSUTF8StringEncoding]);
        }
      }
      [pool drain];
      return str;
    }

    bool Tasklet::setClipboardString(const string& str)
    {
      NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
      NSPasteboard* pboard = [NSPasteboard pasteboardWithName: NSGeneralPboard];
      [pboard declareTypes: [NSArray arrayWithObject: NSStringPboardType] owner: nil];
      NSString* s = [[NSString alloc] initWithUTF8String: str.c_str()];
      bool result = [pboard setString: s forType: NSStringPboardType];
      [s release];
      [pool drain];
      return result;
    }

  }
}

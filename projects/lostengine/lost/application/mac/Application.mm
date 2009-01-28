#include "lost/application/Application.h"

#include <iostream>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"

@interface ApplicationDelegate : NSObject
{
  lost::application::Application* parent;
}
@end

@implementation ApplicationDelegate

- (void)applicationDidFinishLaunching: (NSNotification *)notification
{
  DOUT("applicationDidFinishLaunching");
}

- (void)applicationWillTerminate: (NSNotification *)notification
{
  DOUT("applicationWillTerminate");
  if (parent) parent->quit();
}

- (void)setParent: (lost::application::Application*)newParent
{
  parent = newParent;
}

@end

namespace lost
{
  namespace application
  {
    
    struct Application::ApplicationHiddenMembers
    {
      NSAutoreleasePool*   pool;
      ApplicationDelegate* delegate;
    };

    void Application::initialize()
    {
      DOUT("Application::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new ApplicationHiddenMembers;

      // init pool
      hiddenMembers->pool = [[NSAutoreleasePool alloc] init];

      // init delegate
      hiddenMembers->delegate = [ApplicationDelegate alloc];
      [hiddenMembers->delegate setParent: this];

      // Need this to init AppKit and NSApp
      [[NSApplication sharedApplication] setDelegate: hiddenMembers->delegate];
      [NSBundle loadNibNamed:@"ApplicationMenu" owner:NSApp];
    }

    void Application::finalize()
    {
      DOUT("Application::finalize()");
      [NSApp release];
      [hiddenMembers->delegate release];
      // FIXME: cleanup!
      [hiddenMembers->pool release];
      delete hiddenMembers;
    }

    boost::shared_ptr<Window> Application::createWindow(const std::string& uniqueId, const WindowParams& params)
    {
      DOUT("Application::createWindow()");
      boost::shared_ptr<Window> result(new Window(shared_from_this(), params));
      windows[uniqueId] = result;
      return result;
    }

    void Application::run()
    {
      DOUT("Application::run()");

      boost::function<void (boost::shared_ptr<lost::event::Event>)> f = boost::bind(&RunLoop::quitEventHandler, runLoop.get(), _1);
      addEventListener(lost::application::ApplicationEvent::QUIT(), f);
      // start the main loop
      if(runLoop)
      {
        runLoopThread.reset(new boost::thread(boost::bind(&RunLoop::run, runLoop.get())));
      }
      [NSApp run];
    }

    void Application::quit()
    {
      DOUT("Application::quit()");
      boost::shared_ptr<lost::application::ApplicationEvent> appEvent(new lost::application::ApplicationEvent(""));
      appEvent->type = lost::application::ApplicationEvent::QUIT();
      queueEvent(appEvent);
      if(runLoopThread)
      {
        runLoopThread->join();
      }
    }

  }
}

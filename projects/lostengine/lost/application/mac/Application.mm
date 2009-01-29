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
  if (parent) parent->terminate();
}

- (void)setParent: (lost::application::Application*)newParent
{
  parent = newParent;
}

- (void)terminate
{
  [[NSApplication sharedApplication] terminate: nil];
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

      // TODO: init menu

      // init delegate
      hiddenMembers->delegate = [ApplicationDelegate alloc];
      [hiddenMembers->delegate setParent: this];
      
      // set application delegate
      [[NSApplication sharedApplication] setDelegate: hiddenMembers->delegate];
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
      runLoopThread->run(shared_from_this());
      [NSApp run];
    }

    void Application::quit()
    {
      DOUT("Application::quit()");
      boost::shared_ptr<lost::application::ApplicationEvent> appEvent(new lost::application::ApplicationEvent(""));
      appEvent->type = lost::application::ApplicationEvent::QUIT();
      queueEvent(appEvent);
      [hiddenMembers->delegate performSelectorOnMainThread: @selector(terminate) withObject: nil waitUntilDone: NO];
    }
    
    void Application::terminate()
    {
      DOUT("Application::terminate()");
      runLoopThread->join();
    }
    
  }
}

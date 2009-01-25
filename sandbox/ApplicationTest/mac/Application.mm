#include "../Application.h"

#include <iostream>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"

@interface ApplicationController : NSObject
{
  Application* parent;
}
@end

@implementation ApplicationController

- (void)applicationDidFinishLaunching: (NSNotification *)notification
{
  DOUT("applicationDidFinishLaunching");
}

- (void)applicationWillTerminate: (NSNotification *)notification
{
  DOUT("applicationWillTerminate");
  if (parent) parent->quit();
}

- (void)setParent: (Application*)newParent
{
  parent = newParent;
}

@end

struct Application::ApplicationHiddenMembers
{
  NSAutoreleasePool*     pool;
  ApplicationController* controller;
};

void Application::initialize()
{
  DOUT("Application::initialize()");

  // initialize hiddenMembers
  hiddenMembers = new ApplicationHiddenMembers;

  // init pool
  hiddenMembers->pool = [[NSAutoreleasePool alloc] init];

  // init controller
  hiddenMembers->controller = [ApplicationController alloc];
  [hiddenMembers->controller setParent: this];

  // Need this to init AppKit and NSApp
  [[NSApplication sharedApplication] setDelegate: hiddenMembers->controller];
  [NSBundle loadNibNamed:@"ApplicationMenu" owner:NSApp];
}

void Application::finalize()
{
  DOUT("Application::finalize()");
  [NSApp release];
  [hiddenMembers->pool release];
  delete hiddenMembers;
}

boost::shared_ptr<Window> Application::createWindow(const std::string& uniqueId, const WindowParams& params)
{
  DOUT("Application::createWindow()");
  boost::shared_ptr<Window> result(new Window(params));
  windows[uniqueId] = result;
  return result;
}

void Application::run()
{
  DOUT("Application::run()");

  boost::function<void (boost::shared_ptr<lost::event::Event>)> f = boost::bind(&MainLoop::quitEventHandler, mainLoop.get(), _1);
  addEventListener(lost::application::ApplicationEvent::QUIT(), f);
  // start the main loop
  if(mainLoop)
  {
    mainLoopThread.reset(new boost::thread(boost::bind(&MainLoop::run, mainLoop.get())));
  }
  [NSApp run];
}

void Application::quit()
{
  DOUT("Application::quit()");
  boost::shared_ptr<lost::application::ApplicationEvent> appEvent(new lost::application::ApplicationEvent(""));
  appEvent->type = lost::application::ApplicationEvent::QUIT();
  queueEvent(appEvent);
  if(mainLoopThread)
  {
    mainLoopThread->join();
  }
}

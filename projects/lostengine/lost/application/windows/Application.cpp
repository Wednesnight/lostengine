#include <iostream>

#include "../Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/common/Logger.h"

struct Application::ApplicationHiddenMembers
{
  bool running;
};

void Application::initialize()
{
  DOUT("Application::initialize()");

  // initialize hiddenMembers
  hiddenMembers = new ApplicationHiddenMembers;

/*
  // init pool
  hiddenMembers->pool = [[NSAutoreleasePool alloc] init];

  // init controller
  hiddenMembers->controller = [ApplicationController alloc];
  [hiddenMembers->controller setParent: this];

  // Need this to init AppKit and NSApp
  [[NSApplication sharedApplication] setDelegate: hiddenMembers->controller];
  [NSBundle loadNibNamed:@"ApplicationMenu" owner:NSApp];
*/
}

void Application::finalize()
{
  DOUT("Application::finalize()");
//  [NSApp release];
//  [hiddenMembers->pool release];
  delete hiddenMembers;
}

lost::shared_ptr<Window> Application::createWindow(const std::string& uniqueId, const WindowParams& params)
{
  DOUT("Application::createWindow()");
  lost::shared_ptr<Window> result(new Window(params));
  windows[uniqueId] = result;
  return result;
}

void Application::run()
{
  DOUT("Application::run()");

  boost::function<void (lost::shared_ptr<lost::event::Event>)> f = boost::bind(&MainLoop::quitEventHandler, mainLoop.get(), _1);
  addEventListener(lost::application::ApplicationEvent::QUIT(), f);
  // start the main loop
  if(mainLoop)
  {
    mainLoopThread.reset(new boost::thread(boost::bind(&MainLoop::run, mainLoop.get())));
  }

  MSG msg;
  hiddenMembers->running = true;
  while (hiddenMembers->running)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      switch(msg.message)
      {
        case WM_QUIT:
          hiddenMembers->running = false;
          break;

        default:
          DispatchMessage( &msg );
          break;
      }
    }
  }
  quit();
}

void Application::quit()
{
  DOUT("Application::quit()");
  lost::shared_ptr<lost::application::ApplicationEvent> appEvent(new lost::application::ApplicationEvent(""));
  appEvent->type = lost::application::ApplicationEvent::QUIT();
  queueEvent(appEvent);
  if(mainLoopThread)
  {
    mainLoopThread->join();
  }
}

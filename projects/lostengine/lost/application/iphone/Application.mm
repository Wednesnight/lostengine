#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include "lost/application/Application.h"
#include "lost/common/Logger.h"
#include "lost/application/Tasklet.h"

lost::application::Application* gApplicationPointerForLEApplicationDelegate = NULL;

namespace lost
{
namespace application
{
string Tasklet::getClipboardString()
{
  return string("");
}
bool Tasklet::setClipboardString(const string& str)
{
  return true;
}

void Application::showMouse(bool v) {}

void Application::run(int argc, char *argv[])
{
  DOUT("");
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  UIApplicationMain(argc, argv, @"LEApplication", @"LEApplicationDelegate");
  [pool release];
}

void Application::initialize()
{
  DOUT("");
  gApplicationPointerForLEApplicationDelegate = this;
}


void Application::shutdown()
{
  DOUT("");
}

void Application::processEvents(const ProcessEventPtr& event)
{
  DOUT("");
}

}
}


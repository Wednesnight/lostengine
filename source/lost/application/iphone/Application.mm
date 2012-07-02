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

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "lost/application/iphone/LEApplication.h"
#import "lost/application/iphone/LEApplicationDelegate.h"

#include "lost/application/Application.h"
#include "lost/common/Logger.h"
#include "lost/application/Tasklet.h"

lost::application::Application* gApplicationPointerForLEApplicationDelegate = NULL;

namespace lost
{
namespace application
{

void Application::showMouse(bool v) {}

void Application::run(int argc, char *argv[])
{
  DOUT("");
  NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
  UIApplicationMain(argc, argv, NSStringFromClass([LEApplication class]),
                    NSStringFromClass([LEApplicationDelegate class]));
  [pool release];
}

void Application::initialize()
{
  DOUT("");
  gApplicationPointerForLEApplicationDelegate = this;
}

void Application::finalize()
{
  gApplicationPointerForLEApplicationDelegate = NULL;
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


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

#import "lost/application/mac/ApplicationDelegate.h"
#include <Foundation/NSThread.h>
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Application.h"
#include "lost/event/EventDispatcher.h"

@implementation ApplicationDelegate

- (void) dummyThread:(id)arg;
{
}

-(id)init
{
    if(self = [super init])
    {
      // make this application multithreaded by spawning a dummy thread that exits immediately
      [NSThread detachNewThreadSelector:@selector(dummyThread:) toTarget:self withObject:nil];
    }
    return self;
}

- (void)applicationDidFinishLaunching: (NSNotification *)notification
{
  if (parent)
  {
    lost::shared_ptr<lost::application::ApplicationEvent> event(new lost::application::ApplicationEvent(lost::application::ApplicationEvent::RUN()));
    parent->eventDispatcher->dispatchEvent(event);
  }
}

- (void)applicationWillTerminate: (NSNotification *)notification
{
//  DOUT("applicationWillTerminate");
}

- (void)setParent: (lost::application::Application*)newParent
{
  parent = newParent;
}

- (void)terminate
{
  [[NSApplication sharedApplication] terminate: nil];
}

- (void) quitAction: (id)sender
{
  if (parent)
  {
    parent->quit();
  }
}

- (void)processApplicationEvents
{
  if (parent) parent->eventDispatcher->processEvents();
}

@end

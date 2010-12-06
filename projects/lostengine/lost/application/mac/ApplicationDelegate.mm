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
  DOUT("applicationWillTerminate");
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

#import "lost/application/mac/LEAppDelegate.h"
#include <Foundation/NSThread.h>
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/Application.h"
#include "lost/event/EventDispatcher.h"

@implementation LEAppDelegate

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
  if (application)
  {
    lost::shared_ptr<lost::application::ApplicationEvent> event(new lost::application::ApplicationEvent(lost::application::ApplicationEvent::RUN()));
    application->eventDispatcher->dispatchEvent(event);
  }
}

- (void)applicationWillTerminate: (NSNotification *)notification
{
//  DOUT("applicationWillTerminate");
}

- (void)setApplication:(lost::application::Application*)inApplication
{
  application = inApplication;
}

- (void)terminate
{
  [[NSApplication sharedApplication] terminate: nil];
}

- (void) quitAction: (id)sender
{
  if (application)
  {
    application->quit();
  }
}

- (void)processApplicationEvents
{
  if (application) application->eventDispatcher->processEvents();
}

@end

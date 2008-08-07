#import <UIKit/UIKit.h>
#include "lost/application/iphone/LostApplicationHelpers.h"
#import "lost/application/iphone/LostAppController.h"

void lostApplicationHelpers_run()
{
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	UIApplicationMain(0, NULL, nil, @"LostAppController");
	[pool release];
}

void lostApplicationHelpers_swapBuffers()
{
}

void lostApplicationHelpers_quit()
{
}

void* lostApplicationHelpers_linkHelper()
{
  return (void*)[LostAppController alloc];
}

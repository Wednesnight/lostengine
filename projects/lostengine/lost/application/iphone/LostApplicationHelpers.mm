#import <UIKit/UIKit.h>
#include "lost/application/iphone/LostApplicationHelpers.h"
#import "lost/application/iphone/LostAppController.h"

using namespace std;


void lostApplicationHelpers_runAppController()
{
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
	UIApplicationMain(0, NULL, nil, @"LostAppController");
	[pool release];
}

void lostApplicationHelpers_swapBuffers()
{
}

void lostApplicationHelpers_quitAppController()
{
}

void* lostApplicationHelpers_linkHelper()
{
  return (void*)[LostAppController alloc];
}


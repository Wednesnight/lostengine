#import "LEApplicationDelegate.h"
#import "LEWindow.h"

@implementation LEApplicationDelegate

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
  _window = [[LEWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
}

- (void) applicationWillResignActive:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  NSLog(@"%s", __FUNCTION__);
}

- (void) dealloc
{
  NSLog(@"%s", __FUNCTION__);
  [_window release];
	[super dealloc];
}

@end
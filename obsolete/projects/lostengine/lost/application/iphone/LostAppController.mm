#import "lost/application/iphone/LostAppController.h"
#include "lost/common/Logger.h"
#include "lost/application/iphone/LostApplicationHelpers.h"
#include "lost/platform/Platform.h"

using namespace lost;
using namespace lost::application;
using namespace boost;
using namespace lost::common;

#define kAccelerometerFrequency		100.0 // Hz FIXME: this should really be configurable
LostAppController* lostAppController = NULL; // global instance so LostApplicationHelper functions can find this controller

@implementation LostAppController

- (void)dealloc
{
  if(acevent) { delete acevent; }
	[window release];
	[glView release];
	[super dealloc];
}

- (void)drawView:(LostGlView*)view;
{
}

-(void)setupView:(LostGlView*)view
{
}

- (void)application:(UIApplication *)application didChangeStatusBarFrame:(CGRect)oldStatusBarFrame
{
  DOUT("didChangeStatusBarFrame");
}

- (void)application:(UIApplication *)application didChangeStatusBarOrientation:(UIInterfaceOrientation)oldStatusBarOrientation
{
  DOUT("didChangeStatusBarOrientation");
}

- (void)application:(UIApplication *)application willChangeStatusBarFrame:(CGRect)newStatusBarFrame
{
  DOUT("willChangeStatusBarFrame");
}

- (void)application:(UIApplication *)application willChangeStatusBarOrientation:(UIInterfaceOrientation)newStatusBarOrientation duration:(NSTimeInterval)duration
{
  DOUT("willChangeStatusBarOrientation");
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
  DOUT("applicationDidBecomeActive");
}

- (void)applicationDidFinishLaunching:(UIApplication*)application
{
  DOUT("applicationDidFinishLaunching");
  lostAppController = self; // set up global instance pointer so LostApplicationHelper functions can find us
  // setup accelerometer event instance.
  // the crude pointer-pointer construct is required since C++ constructors aren't called in Objective-C++ files
  // we therefore have to allocate the shared_ptr by hand, then the actual event instance
  acevent = new shared_ptr<AccelerometerEvent>;
  acevent->reset(new AccelerometerEvent(AccelerometerEvent::DEVICE_ACCELERATED()));
  
	CGRect					rect = [[UIScreen mainScreen] bounds];
	
	//Create a full-screen window
	window = [[UIWindow alloc] initWithFrame:rect];
	
	//Create the OpenGL ES view and add it to the window
	glView = [[LostGlView alloc] initWithFrame:rect];
	[window addSubview:glView];

	glView.delegate = self;

	//Show the window
	[window makeKeyAndVisible];
	
	//Configure and start accelerometer
//	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
//	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
  lostApplicationHelpers_preinit();
  lostApplicationHelpers_init();
  lostApplicationHelpers_resize(rect.size.width, rect.size.height);
  lostApplicationHelpers_run();
}

- (void) swapBuffers
{
  [glView swapBuffers];
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
  DOUT("applicationDidReceiveMemoryWarning");
}

- (void)applicationWillResignActive:(UIApplication *)application
{
  DOUT("applicationWillResignActive");
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  DOUT("applicationWillTerminate");
  lostApplicationHelpers_quit();
}


- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
  (*acevent)->timeStamp = acceleration.timestamp;
  (*acevent)->x = acceleration.x;
  (*acevent)->y = acceleration.y;
  (*acevent)->z = acceleration.z;
  lostApplicationHelpers_dispatchEvent(*acevent);
}

@end

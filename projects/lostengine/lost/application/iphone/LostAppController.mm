#import "lost/application/iphone/LostAppController.h"
#include "lost/common/Logger.h"
#include "lost/application/iphone/LostApplicationHelpers.h"
#include "lost/platform/Platform.h"

using namespace lost;



using namespace lost::common;

// CONSTANTS
//#define kTeapotScale				3.0
#define kAccelerometerFrequency		100.0 // Hz
//#define kRenderingFrequency			30.0 // Hz
//#define kFilteringFactor			0.1

// MACROS
//#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)

//#define OLD_DRAW 0

LostAppController* lostAppController = NULL;


@implementation LostAppController

- (void)dealloc
{
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
  NSLog(@"didChangeStatusBarFrame");
}

- (void)application:(UIApplication *)application didChangeStatusBarOrientation:(UIInterfaceOrientation)oldStatusBarOrientation
{
  NSLog(@"didChangeStatusBarOrientation");
}

- (void)application:(UIApplication *)application willChangeStatusBarFrame:(CGRect)newStatusBarFrame
{
  NSLog(@"willChangeStatusBarFrame");
}

- (void)application:(UIApplication *)application willChangeStatusBarOrientation:(UIInterfaceOrientation)newStatusBarOrientation duration:(NSTimeInterval)duration
{
  NSLog(@"willChangeStatusBarOrientation");
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
  NSLog(@"applicationDidBecomeActive");
}

- (void)applicationDidFinishLaunching:(UIApplication*)application
{
  NSLog(@"applicationDidFinishLaunching");
  lostAppController = self;
  
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
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
  lostApplicationHelpers_preinit();
  lostApplicationHelpers_resize(rect.size.width, rect.size.height);
  lostApplicationHelpers_init();
  lostApplicationHelpers_run();
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
  NSLog(@"applicationDidReceiveMemoryWarning");
}

- (void)applicationWillResignActive:(UIApplication *)application
{
  NSLog(@"applicationWillResignActive");
}

- (void)applicationWillTerminate:(UIApplication *)application
{
  NSLog(@"applicationWillTerminate");
  lostApplicationHelpers_quit();
}


- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
  // FIXME: pass on accelerometer data as events
}

- (void) swapBuffers
{
  [glView swapBuffers];
}

@end

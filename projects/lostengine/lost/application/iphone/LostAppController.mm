#import "lost/application/iphone/LostAppController.h"
#import "lost/application/iphone/teapot.h"
#include "lost/common/Logger.h"
#include "lost/application/iphone/LostApplicationHelpers.h"
#include "lost/gl/Draw.h"
#include "lost/platform/Platform.h"
#include "lost/math/AABB.h"

using namespace lost;
using namespace lost::math;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::common;

// CONSTANTS
#define kTeapotScale				3.0
#define kAccelerometerFrequency		100.0 // Hz
#define kRenderingFrequency			30.0 // Hz
#define kFilteringFactor			0.1

// MACROS
#define DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) / 180.0 * M_PI)

#define OLD_DRAW 0



@implementation LostAppController

- (void)drawView:(LostGlView*)view;
{
  DOUT("idle");
  int width = 320;
  int height = 480;
  
  glViewport(0, 0, width, height);GLDEBUG;
  
  glDisable(GL_DEPTH_TEST);GLDEBUG;
  glDisable(GL_TEXTURE_2D);GLDEBUG;
  glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
  
  set3DProjection(width, height, Vec3(10,10,10), Vec3(0,0,0), Vec3(0,1,0), 120, .1, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  setColor(whiteColor);
  AABB box(Vec3(-2,-2,-2), Vec3(4,4,4));
  drawAABB(box);
  
  drawAxes(Vec3(10,10,10));
  
  set2DProjection(Vec2(0,0), Vec2(width, height));
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  double now = platform::currentTimeSeconds();
  double delta = now-lastTime;
  fpsMeter->render(5,5,delta);
  lastTime = now;
}

-(void)setupView:(LostGlView*)view
{
  glEnableClientState(GL_VERTEX_ARRAY);
  lastTime = platform::currentTimeSeconds();
  fpsMeter = new lost::common::FpsMeter();    
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

	CGRect					rect = [[UIScreen mainScreen] bounds];
	
	//Create a full-screen window
	window = [[UIWindow alloc] initWithFrame:rect];
	
	//Create the OpenGL ES view and add it to the window
	LostGlView *glView = [[LostGlView alloc] initWithFrame:rect];
	[window addSubview:glView];

	glView.delegate = self;
	glView.animationInterval = 1.0 / kRenderingFrequency;
	[glView startAnimation];

	[glView release];
	
	//Show the window
	[window makeKeyAndVisible];
	
	//Configure and start accelerometer
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
  lostApplicationHelpers_preinit();
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


- (void)dealloc
{
	[window release];
	[super dealloc];
}

- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	//Use a basic low-pass filter to only keep the gravity in the accelerometer values
	accel[0] = acceleration.x * kFilteringFactor + accel[0] * (1.0 - kFilteringFactor);
	accel[1] = acceleration.y * kFilteringFactor + accel[1] * (1.0 - kFilteringFactor);
	accel[2] = acceleration.z * kFilteringFactor + accel[2] * (1.0 - kFilteringFactor);
}

@end

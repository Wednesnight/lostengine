//
//  gltestAppDelegate.m
//  gltest
//
//  Created by Tony Kostanjsek on 19.02.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "gltestAppDelegate.h"
#import "EAGLView.h"

@implementation gltestAppDelegate

@synthesize window;
@synthesize glView;

- (void) applicationDidFinishLaunching:(UIApplication *)application
{
	[glView startAnimation];
  CGRect b = [UIScreen mainScreen].bounds;
  NSLog(@"screen: %.2f %.2f %.2f %.2f", b.origin.x, b.origin.y, b.size.width, b.size.height);
}

- (void) applicationWillResignActive:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
	[glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	[glView stopAnimation];
}

- (void) dealloc
{
	[window release];
	[glView release];
	
	[super dealloc];
}

@end

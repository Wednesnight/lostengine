//
//  LostEngineMinimalAppAppDelegate.m
//  LostEngineMinimalApp
//
//  Created by Tony Kostanjsek on 29.07.08.
//  Copyright Orbster GmbH 2008. All rights reserved.
//

#import "LostEngineMinimalAppAppDelegate.h"
#import "EAGLView.h"

@implementation LostEngineMinimalAppAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {

	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}

- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end

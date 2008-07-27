//
//  iphonetestAppDelegate.m
//  iphonetest
//
//  Created by Tony Kostanjsek on 27.07.08.
//  Copyright Orbster GmbH 2008. All rights reserved.
//

#import "iphonetestAppDelegate.h"

@implementation iphonetestAppDelegate

@synthesize window;

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	
	// Override point for customization after app launch	
    [window makeKeyAndVisible];
}


- (void)dealloc {
	[window release];
	[super dealloc];
}


@end

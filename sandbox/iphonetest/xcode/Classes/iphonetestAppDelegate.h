//
//  iphonetestAppDelegate.h
//  iphonetest
//
//  Created by Tony Kostanjsek on 27.07.08.
//  Copyright Orbster GmbH 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class iphonetestViewController;

@interface iphonetestAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
}

@property (nonatomic, retain) UIWindow *window;

@end


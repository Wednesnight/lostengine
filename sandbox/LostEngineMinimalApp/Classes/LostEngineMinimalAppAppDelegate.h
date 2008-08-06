//
//  LostEngineMinimalAppAppDelegate.h
//  LostEngineMinimalApp
//
//  Created by Tony Kostanjsek on 29.07.08.
//  Copyright Orbster GmbH 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface LostEngineMinimalAppAppDelegate : NSObject <UIApplicationDelegate> {
	IBOutlet UIWindow *window;
	IBOutlet EAGLView *glView;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) EAGLView *glView;

@end


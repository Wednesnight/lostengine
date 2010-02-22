//
//  main.m
//  gltest
//
//  Created by Tony Kostanjsek on 19.02.10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, @"LEApplication", @"LEApplicationDelegate");
    [pool release];
    return retVal;
}

//
//  main.m
//  iphonetest
//
//  Created by Tony Kostanjsek on 27.07.08.
//  Copyright Orbster GmbH 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <iostream>
#include "lost/common/Logger.h"

using namespace std;
using namespace lost;
using namespace boost;
using namespace lost::platform;
using namespace lost::common;

#include "helper.h"

int main(int argc, char *argv[]) {

  LogLevel( log_all );
  testit();
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = UIApplicationMain(argc, argv, nil, nil);
	[pool release];
  cout << "shutting down main" << endl;
	return retVal;
}

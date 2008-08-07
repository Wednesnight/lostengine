#import "LostGlView.h"

@interface LostAppController : NSObject <UIAccelerometerDelegate, LostGlViewDelegate>
{
	UIWindow*				window;
	UIAccelerationValue		accel[3];
}

@end

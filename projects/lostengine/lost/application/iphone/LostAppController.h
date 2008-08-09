#import "LostGlView.h"

@interface LostAppController : NSObject <UIAccelerometerDelegate, LostGlViewDelegate>
{
	UIWindow*				window;
	UIAccelerationValue		accel[3];
  
  double lastTime;
  LostGlView* glView;
}

- (void) swapBuffers;

@end

extern LostAppController* lostAppController;

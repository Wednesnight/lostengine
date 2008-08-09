#import "LostGlView.h"
#include "lost/common/FpsMeter.h"

@interface LostAppController : NSObject <UIAccelerometerDelegate, LostGlViewDelegate>
{
	UIWindow*				window;
	UIAccelerationValue		accel[3];
  
  lost::common::FpsMeter* fpsMeter;
  double lastTime;
}

@end

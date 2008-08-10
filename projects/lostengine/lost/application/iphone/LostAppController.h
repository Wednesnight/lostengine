#import "LostGlView.h"
#include <boost/shared_ptr.hpp>
#include "lost/application/AccelerometerEvent.h"

@interface LostAppController : NSObject <UIAccelerometerDelegate, LostGlViewDelegate>
{
	UIWindow*             window;
	UIAccelerationValue		accel[3];
  
  double lastTime;
  LostGlView* glView;
  boost::shared_ptr<lost::application::AccelerometerEvent>* acevent;
}

- (void) swapBuffers;

@end

extern LostAppController* lostAppController;

@class LEWindow;
@class CADisplayLink;

@interface LEApplicationDelegate : NSObject
{
  LEWindow* _window;
  CADisplayLink* _displayLink;
}

@end
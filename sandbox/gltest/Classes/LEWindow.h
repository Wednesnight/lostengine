@class LEGLView;

@interface LEWindow : UIWindow
{
  LEGLView* _leglView;
}

@property(nonatomic, readonly) LEGLView* leglView;

@end
#import "LEWindow.h"
#import "LEGLView.h"

@implementation LEWindow

@synthesize
leglView = _leglView
;

-(id)initWithFrame:(CGRect)fr
{
  NSLog(@"building window");
  if(self = [super initWithFrame:fr])
  {
    _leglView = [[LEGLView alloc] initWithFrame:fr];
    [self addSubview:_leglView];
  }
  return self;
}

-(void)dealloc
{
  [_leglView dealloc];
  [super dealloc];
}

@end
#import "LEWindow.h"
#import "LEGLView.h"
#import "lost/common/Logger.h"

@implementation LEWindow

@synthesize
leglView = _leglView
;

-(id)initWithFrame:(CGRect)fr
{
  DOUT("building window");
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
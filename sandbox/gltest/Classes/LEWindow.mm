#import "LEWindow.h"
#import "LEGLView.h"

@implementation LEWindow

@synthesize
leglView = _leglView
;

-(id)initWithFrame:(CGRect)fr
{
  if(self = [super initWithFrame:fr])
  {
    _leglView = [[LEGLView alloc] initWithFrame:fr];
  }
  return self;
}

-(void)dealloc
{
  [_leglView dealloc];
  [super dealloc];
}

@end
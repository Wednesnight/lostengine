#import "lost/application/mac/LEGLView.h"
#import <AppKit/NSOpenGL.h>

@implementation LEGLView

- (id)initWithFrame: (NSRect)frame
{
  NSOpenGLPixelFormatAttribute windowedAttributes[] =
  {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, [[[NSUserDefaults standardUserDefaults] objectForKey:@"colorDepth"] intValue],
    NSOpenGLPFAAlphaSize, 8,
    NSOpenGLPFADepthSize, 32,
    0
  };
  NSOpenGLPixelFormat* pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:windowedAttributes] autorelease];
  return [super initWithFrame:frame pixelFormat:pixelFormat];
}

@end

#import "lost/application/mac/GLView.h"
#import <AppKit/NSOpenGL.h>
#import "lost/application/mac/ApplicationWindow.h"
#include "lost/application/MouseEvent.h"

@implementation GLView

- (id)initWithFrame: (NSRect)frame
{
  NSOpenGLPixelFormatAttribute windowedAttributes[] =
  {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, [[[NSUserDefaults standardUserDefaults] objectForKey:@"colorDepth"] intValue],
    NSOpenGLPFAAlphaSize, 16,
    NSOpenGLPFADepthSize, 32,
    0
  };
  return [super initWithFrame: frame pixelFormat: [[NSOpenGLPixelFormat alloc] initWithAttributes: windowedAttributes]];
}

@end

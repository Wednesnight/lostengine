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

- (id)window
{
  return (ApplicationWindow*)[super window];
}

- (void)mouseDown: (NSEvent*)event
{
  [((ApplicationWindow*)[self window]) mouseEvent: event type: lost::application::MouseEvent::MOUSE_DOWN().c_str()];
}

- (void)mouseUp: (NSEvent*)event
{
  [((ApplicationWindow*)[self window]) mouseEvent: event type: lost::application::MouseEvent::MOUSE_UP().c_str()];
}

- (void)rightMouseDown: (NSEvent*)event
{
  [((ApplicationWindow*)[self window]) mouseEvent: event type: lost::application::MouseEvent::MOUSE_DOWN().c_str()];
}

- (void)rightMouseUp: (NSEvent*)event
{
  [((ApplicationWindow*)[self window]) mouseEvent: event type: lost::application::MouseEvent::MOUSE_UP().c_str()];
}

- (void)scrollWheel: (NSEvent*)event
{
  [((ApplicationWindow*)[self window]) mouseEvent: event type: lost::application::MouseEvent::MOUSE_SCROLL().c_str()];
}

@end

#include "../Window.h"

#import <AGL/AGL.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "lost/common/Logger.h"

@interface GLView : NSOpenGLView
{
}
@end

@implementation GLView

- (id)initWithFrame: (NSRect)frame
{
  NSOpenGLPixelFormatAttribute windowedAttributes[] =
  {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFAColorSize, [[[NSUserDefaults standardUserDefaults] objectForKey:@"colorDepth"] intValue],
    NSOpenGLPFAAlphaSize, 16,
    NSOpenGLPFADepthSize, 16,
    0
  };
  return [super initWithFrame: frame pixelFormat: [[NSOpenGLPixelFormat alloc] initWithAttributes: windowedAttributes]];
}

//- (void)drawRect: (NSRect)frame
//{
//}

@end

@interface ApplicationWindow : NSWindow
{
  Window* parent;
}
@end

@implementation ApplicationWindow

- (id)initWithContentRect: (NSRect)contentRect styleMask: (NSUInteger)aStyle backing: (NSBackingStoreType)bufferingType defer: (BOOL)flag
{
	self = [super initWithContentRect: contentRect styleMask: aStyle backing: bufferingType defer: flag];
	if(self != nil)
	{
    //
	}
	return self;
}
//- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen;

- (void) windowWillClose: (NSNotification *)notification
{
  if (parent) parent->close();
}

- (void)setParent: (Window*)newParent
{
  parent = newParent;
}

- (void)keyDown: (NSEvent*)event
{
  DOUT("keyDown: " << [event keyCode]);
}

- (void)keyUp: (NSEvent*)event
{
  DOUT("keyUp: " << [event keyCode]);
}

@end

struct Window::WindowHiddenMembers
{
  ApplicationWindow* window;
  GLView*            view;
};

void Window::initialize()
{
  DOUT("Window::initialize()");

  // initialize hiddenMembers
  hiddenMembers = new WindowHiddenMembers;

  // create view
  hiddenMembers->view = [[GLView alloc] initWithFrame: NSMakeRect(params.position.x, params.position.y, params.size.width, params.size.height)];

  // create window
  hiddenMembers->window = [[ApplicationWindow alloc]
                           initWithContentRect: [hiddenMembers->view frame]
                           styleMask: NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask
                           backing: NSBackingStoreBuffered
                           defer: NO];
  // set params
  [hiddenMembers->window setParent: this];
  [hiddenMembers->window setTitle: [[NSString alloc] initWithCString:params.caption.c_str()]];
//  [hiddenMembers->window center];
  [hiddenMembers->window setContentView: hiddenMembers->view];
  [hiddenMembers->window setDelegate: hiddenMembers->window];
  [hiddenMembers->window makeKeyAndOrderFront: nil];

  [[hiddenMembers->view openGLContext] makeCurrentContext];
  context.reset(new Context);
}

void Window::finalize()
{
  DOUT("Window::finalize()");
  // FIXME: cleanup!
  [hiddenMembers->view dealloc];
//  [hiddenMembers->window dealloc];
  delete hiddenMembers;
}

void Window::close()
{
  DOUT("Window::close()");
}

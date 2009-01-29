#include "lost/application/Window.h"

#import <AGL/AGL.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"

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

@end

@interface ApplicationWindow : NSWindow
{
  lost::application::Window* parent;
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

- (BOOL)isReleasedWhenClosed
{
  return NO;
}

- (BOOL)windowShouldClose: (id)window
{
  // TODO: add windowShouldClose event
  return YES;
}

- (void) windowWillClose: (NSNotification *)notification
{
  // TODO: add windowWillClose event
}

- (void)setParent: (lost::application::Window*)newParent
{
  parent = newParent;
}

- (void)keyDown: (NSEvent*)event
{
  if (parent)
  {
    boost::shared_ptr<lost::application::KeyEvent> keyEvent(new lost::application::KeyEvent(lost::application::KeyEvent::KEY_DOWN()));
    keyEvent->key       = [event keyCode];
    // TODO: UTF character
  //  keyEvent->character = *[[event characters] UTF8String];
    keyEvent->pressed   = true;
    keyEvent->repeat    = [event isARepeat];
    parent->dispatcher->queueEvent(keyEvent);
  }  
}

- (void)keyUp: (NSEvent*)event
{
  DOUT("keyUp: " << [event keyCode] <<", "<< [[event characters] UTF8String] <<", "<< (bool)[event isARepeat]);
}

@end

namespace lost
{
  namespace application
  {
        
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

      [[hiddenMembers->view openGLContext] makeCurrentContext];
      context.reset(new gl::Context);
      
      open();
    }

    void Window::finalize()
    {
      DOUT("Window::finalize()");
      // FIXME: cleanup!
      [hiddenMembers->view release];
      [hiddenMembers->window release];
      delete hiddenMembers;
    }

    void Window::open()
    {
      [hiddenMembers->window makeKeyAndOrderFront: nil];
    }
    
    void Window::close()
    {
      [hiddenMembers->window performClose: nil];
    }

  }
}

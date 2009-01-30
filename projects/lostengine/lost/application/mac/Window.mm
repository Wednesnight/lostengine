#include "lost/application/Window.h"

#import <AGL/AGL.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeyCode.h"

@interface ApplicationWindow : NSWindow
{
  lost::application::Window* parent;

  bool leftButtonDown;
  bool rightButtonDown;
}
@end

@interface GLView : NSOpenGLView
{
}
@end

@implementation ApplicationWindow

- (id)initWithContentRect: (NSRect)contentRect styleMask: (NSUInteger)aStyle backing: (NSBackingStoreType)bufferingType defer: (BOOL)flag
{
	self = [super initWithContentRect: contentRect styleMask: aStyle backing: bufferingType defer: flag];
	if(self != nil)
	{
    // TODO: init window params
	}
	return self;
}
// TODO: is screen of any use for us?
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

- (GLView*)contentView
{
  return (GLView*)[super contentView];
}

- (lost::application::KeyCode)translateKeyCode: (NSInteger)nativeKeyCode
{
  switch (nativeKeyCode)
  {
    case 53 : return lost::application::K_ESCAPE;
    default : return lost::application::K_UNKNOWN;
  }
}

- (void)keyEvent: (NSEvent*)event type:(std::string)type pressed:(BOOL)pressed
{
  // FIXME: virtual keycode translation!
  if (parent)
  {
    boost::shared_ptr<lost::application::KeyEvent> keyEvent(new lost::application::KeyEvent(type));
    keyEvent->window    = parent->shared_from_this();
    keyEvent->key       = [self translateKeyCode: [event keyCode]];
    // TODO: UTF character
    keyEvent->character = [[event characters] UTF8String];
    keyEvent->pressed   = pressed;
    keyEvent->repeat    = [event isARepeat];
    parent->dispatcher->queueEvent(keyEvent);
  }  
}

- (void)keyDown: (NSEvent*)event
{
  [self keyEvent: event type: lost::application::KeyEvent::KEY_DOWN() pressed: YES];
}

- (void)keyUp: (NSEvent*)event
{
  [self keyEvent: event type: lost::application::KeyEvent::KEY_UP() pressed: NO];
}

- (lost::application::MouseButton)translateMouseButton: (NSInteger)nativeButton
{
  switch (nativeButton)
  {
    case 0  : return lost::application::MB_LEFT;
    case 1  : return lost::application::MB_RIGHT;
    default : return lost::application::MB_UNKNOWN;
  }
}

- (bool)mouseButtonPressed: (std::string)eventType button: (lost::application::MouseButton)button
{
  if (eventType == lost::application::MouseEvent::MOUSE_DOWN())
  {
    if (button == lost::application::MB_LEFT) leftButtonDown = true;
      else if (button == lost::application::MB_RIGHT) rightButtonDown = true;
  }
  else if (eventType == lost::application::MouseEvent::MOUSE_UP())
  {
    if (button == lost::application::MB_LEFT) leftButtonDown = false;
      else if (button == lost::application::MB_RIGHT) rightButtonDown = false;
  }

  switch (button)
  {
    case lost::application::MB_LEFT  : return leftButtonDown;
    case lost::application::MB_RIGHT : return rightButtonDown;
    default                          : return false;
  }
}

- (void)mouseEvent: (NSEvent*)event type:(std::string)type
{
  if (parent)
  {
    boost::shared_ptr<lost::application::MouseEvent> mouseEvent(new lost::application::MouseEvent(type));
    NSPoint rel = [self mouseLocationOutsideOfEventStream];
    NSPoint abs = [NSEvent mouseLocation];
    mouseEvent->window  = parent->shared_from_this();
    mouseEvent->pos     = lost::math::Vec2(rel.x, rel.y);
    mouseEvent->absPos  = lost::math::Vec2(abs.x, abs.y);
    mouseEvent->button  = [self translateMouseButton: [event buttonNumber]];
    mouseEvent->pressed = [self mouseButtonPressed: type button: mouseEvent->button];
    parent->dispatcher->queueEvent(mouseEvent);
  }  
}

- (void)mouseMoved: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_MOVE()];
}

- (void)mouseDragged: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_MOVE()];
}

- (void)rightMouseDragged: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_MOVE()];
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

- (id)window
{
  return (ApplicationWindow*)[super window];
}

- (void)mouseDown: (NSEvent*)event
{
  [[self window] mouseEvent: event type: lost::application::MouseEvent::MOUSE_DOWN()];
}

- (void)mouseUp: (NSEvent*)event
{
  [[self window] mouseEvent: event type: lost::application::MouseEvent::MOUSE_UP()];
}

- (void)rightMouseDown: (NSEvent*)event
{
  [[self window] mouseEvent: event type: lost::application::MouseEvent::MOUSE_DOWN()];
}

- (void)rightMouseUp: (NSEvent*)event
{
  [[self window] mouseEvent: event type: lost::application::MouseEvent::MOUSE_UP()];
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
      [hiddenMembers->window setAcceptsMouseMovedEvents: YES];
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

#import "lost/application/mac/ApplicationWindow.h"
#import <AppKit/NSPasteBoard.h>
#import <AppKit/NSDragging.h>
#import <AppKit/NSEvent.h>
#include "lost/application/Window.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/ResizeEvent.h"
#import "lost/application/mac/GLView.h"
#include "lost/application/KeyCode.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseButton.h"
#include "lost/application/MouseEvent.h"
#include "lost/event/EventDispatcher.h"
#include "lost/common/Logger.h"

@implementation ApplicationWindow

- (id)initWithContentRect: (NSRect)contentRect styleMask: (NSUInteger)aStyle backing: (NSBackingStoreType)bufferingType defer: (BOOL)flag
{
	self = [super initWithContentRect: contentRect styleMask: aStyle backing: bufferingType defer: flag];
	if(self != nil)
	{
    [self registerForDraggedTypes:[NSArray arrayWithObject:NSURLPboardType]];
	}
	return self;
}
// TODO: is screen of any use for us?
//- (id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)aStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)flag screen:(NSScreen *)screen;
- (void) dispatchDropEvents:(id <NSDraggingInfo>)sender 
{
  NSPasteboard* pboard = [sender draggingPasteboard];
  if(!([[pboard types] containsObject:NSURLPboardType] && parent)) return;
    
  NSArray *classes = [[NSArray alloc] initWithObjects:[NSURL class], nil];
  NSDictionary *options = [NSDictionary dictionary];
  NSArray *copiedItems = [pboard readObjectsForClasses:classes options:options];
  lost::application::DragNDropEventPtr dragNDropEvent(new lost::application::DragNDropEvent(lost::application::DragNDropEvent::DROP()));
  NSPoint rel = [self mouseLocationOutsideOfEventStream];
  NSPoint abs = [NSEvent mouseLocation];
  dragNDropEvent->window  = parent;
  dragNDropEvent->pos     = lost::math::Vec2(rel.x, rel.y);
  dragNDropEvent->absPos  = lost::math::Vec2(abs.x, abs.y);
  
  for(NSURL* fileURL in copiedItems)
  {
    NSString* relativePath = [fileURL relativePath];
    const char* cString = [relativePath cStringUsingEncoding: NSUTF8StringEncoding];
    dragNDropEvent->addPath(cString);
  }
  parent->dispatcher->queueEvent(dragNDropEvent);
}

/*- (void) dispatchDragNDropEvent:(id <NSDraggingInfo>)sender type:(string)type
{
  NSPasteboard* pboard = [sender draggingPasteboard];
  NSArray* items = [pboard pasteboardItems];
  int count = [items count];
  if ([[pboard types] containsObject:NSURLPboardType] && parent)
  {
    NSURL *fileURL = [NSURL URLFromPasteboard:pboard];
    NSString* relativePath = [fileURL relativePath];
    const char* cString = [relativePath cStringUsingEncoding: NSUTF8StringEncoding];
    string filename(cString);
    lost::application::DragNDropEventPtr dragNDropEvent(new lost::application::DragNDropEvent(type, filename));
    NSPoint rel = [self mouseLocationOutsideOfEventStream];
    NSPoint abs = [NSEvent mouseLocation];
    dragNDropEvent->window  = parent;
    dragNDropEvent->pos     = lost::math::Vec2(rel.x, rel.y);
    dragNDropEvent->absPos  = lost::math::Vec2(abs.x, abs.y);
    parent->dispatcher->queueEvent(dragNDropEvent);
  }
}*/

- (BOOL) performDragOperation:(id <NSDraggingInfo>)sender
{
  [self dispatchDropEvents:sender];
  return YES;
}

- (NSDragOperation) draggingEntered:(id <NSDraggingInfo>)sender
{
//  [self dispatchDragNDropEvent: sender type: lost::application::DragNDropEvent::DRAG_ENTER()];
  return NSDragOperationLink;
}

- (void) draggingExited:(id <NSDraggingInfo>)sender
{
//  [self dispatchDragNDropEvent: sender type: lost::application::DragNDropEvent::DRAG_LEAVE()];
}

- (NSDragOperation) draggingUpdated:(id <NSDraggingInfo>)sender
{
  NSPasteboard* pboard = [sender draggingPasteboard];
  if ([[pboard types] containsObject:NSURLPboardType])
  {
//    [self dispatchDragNDropEvent: sender type: lost::application::DragNDropEvent::DRAG_UPDATE()];
    return NSDragOperationLink;
  }
  else
  {
    // only accept URLs
    return NSDragOperationNone;
  }
}

- (BOOL)windowShouldClose: (id)window
{
  // TODO: add windowShouldClose event
  return YES;
}

- (void) windowWillClose: (NSNotification *)notification
{
  if (parent)
  {
    lost::shared_ptr<lost::application::WindowEvent> windowEvent(new lost::application::WindowEvent(
        lost::application::WindowEvent::CLOSE(), parent));
    parent->dispatcher->queueEvent(windowEvent);
  }
}

- (void)windowDidResize:(NSNotification *)notification
{
  if (parent)
  {
    NSRect curFrame = [self contentRectForFrameRect:[self frame]];
    lost::shared_ptr<lost::application::ResizeEvent> resizeEvent(new lost::application::ResizeEvent(curFrame.size.width, curFrame.size.height));
    parent->dispatcher->queueEvent(resizeEvent);
  }
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
    case  0 : return lost::application::K_A;
    case 11 : return lost::application::K_B;
    case  8 : return lost::application::K_C;
    case  2 : return lost::application::K_D;
    case 14 : return lost::application::K_E;
    case  3 : return lost::application::K_F;
    case  5 : return lost::application::K_G;
    case  4 : return lost::application::K_H;
    case 34 : return lost::application::K_I;
    case 38 : return lost::application::K_J;
    case 40 : return lost::application::K_K;
    case 37 : return lost::application::K_L;
    case 46 : return lost::application::K_M;
    case 45 : return lost::application::K_N;
    case 31 : return lost::application::K_O;
    case 35 : return lost::application::K_P;
    case 12 : return lost::application::K_Q;
    case 15 : return lost::application::K_R;
    case  1 : return lost::application::K_S;
    case 17 : return lost::application::K_T;
    case 32 : return lost::application::K_U;
    case  9 : return lost::application::K_V;
    case 13 : return lost::application::K_W;
    case  7 : return lost::application::K_X;
    case  6 : return lost::application::K_Y;
    case 16 : return lost::application::K_Z;

    case 18 : return lost::application::K_1;
    case 19 : return lost::application::K_2;
    case 20 : return lost::application::K_3;
    case 21 : return lost::application::K_4;
    case 23 : return lost::application::K_5;
    case 22 : return lost::application::K_6;
    case 26 : return lost::application::K_7;
    case 28 : return lost::application::K_8;
    case 25 : return lost::application::K_9;
    case 29 : return lost::application::K_0;

    case 10 : return lost::application::K_CIRCUMFLEX;
    case 27 : return lost::application::K_HELP;
    case 24 : return lost::application::K_APOSTROPHE;

    case 36 : return lost::application::K_ENTER;
    case 48 : return lost::application::K_TAB;
    case 49 : return lost::application::K_SPACE;
    case 51 : return lost::application::K_BACKSPACE;
    case 53 : return lost::application::K_ESCAPE;
    case 117 : return lost::application::K_DELETE;

    case 123 : return lost::application::K_LEFT;
    case 124 : return lost::application::K_RIGHT;
    case 126 : return lost::application::K_UP;
    case 125 : return lost::application::K_DOWN;
    case 116 : return lost::application::K_PAGEUP;
    case 121 : return lost::application::K_PAGEDOWN;
    case 115 : return lost::application::K_HOME;
    case 119 : return lost::application::K_END;
      
    case 122 : return lost::application::K_F1;
    case 120 : return lost::application::K_F2;
    case  99 : return lost::application::K_F3;
    case 118 : return lost::application::K_F4;
    case  96 : return lost::application::K_F5;
    case  97 : return lost::application::K_F6;
    case  98 : return lost::application::K_F7;
    case 100 : return lost::application::K_F8;
    case 101 : return lost::application::K_F9;
    case 109 : return lost::application::K_F10;
    case 103 : return lost::application::K_F11;
    case 111 : return lost::application::K_F12;

    default : return lost::application::K_UNKNOWN;
  }
}

- (void)keyEvent: (NSEvent*)event type:(lost::event::Type)type pressed:(BOOL)pressed
{
  // FIXME: virtual keycode translation!
  if (parent)
  {
    lost::shared_ptr<lost::application::KeyEvent> keyEvent(new lost::application::KeyEvent(type));
    keyEvent->window      = parent;
    keyEvent->key         = [self translateKeyCode: [event keyCode]];
    keyEvent->ctrlDown    = ([event modifierFlags] & NSControlKeyMask);
    keyEvent->shiftDown   = ([event modifierFlags] & NSShiftKeyMask);
    keyEvent->altDown     = ([event modifierFlags] & NSAlternateKeyMask);
    keyEvent->specialDown = ([event modifierFlags] & NSCommandKeyMask);
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

- (bool)mouseButtonPressed: (lost::event::Type)eventType button: (lost::application::MouseButton)button
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

- (void)mouseEvent: (NSEvent*)event type:(lost::event::Type)type
{
  if (parent)
  {
    lost::shared_ptr<lost::application::MouseEvent> mouseEvent(new lost::application::MouseEvent(type));
    NSPoint rel = [self mouseLocationOutsideOfEventStream];
    NSPoint abs = [NSEvent mouseLocation];
    mouseEvent->window  = parent;
    mouseEvent->pos     = lost::math::Vec2(rel.x, rel.y);
    mouseEvent->absPos  = lost::math::Vec2(abs.x, abs.y);
    mouseEvent->button  = [self translateMouseButton: [event buttonNumber]];
    mouseEvent->pressed = [self mouseButtonPressed: type button: mouseEvent->button];
    mouseEvent->scrollDelta = lost::math::Vec3([event deltaX], [event deltaY], [event deltaZ]);
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

- (void)mouseDown: (NSEvent*)event
{
    [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_DOWN()];
}

- (void)mouseUp: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_UP()];
}

- (void)rightMouseDown: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_DOWN()];
}

- (void)rightMouseUp: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_UP()];
}

- (void)scrollWheel: (NSEvent*)event
{
  [self mouseEvent: event type: lost::application::MouseEvent::MOUSE_SCROLL()];
}


@end


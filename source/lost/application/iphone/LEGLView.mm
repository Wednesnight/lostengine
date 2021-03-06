/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#import "LEGLView.h"
#import <QuartzCore/CAEAGLLayer.h>
#import "lost/gl/Utils.h"
#import "lost/common/Logger.h"
#include "lost/application/MouseButton.h"
#include "lost/application/MouseEvent.h"
#include "lost/event/EventDispatcher.h"

@implementation LEGLView

@synthesize
context = _context,
backingWidth = _backingWidth,
backingHeight = _backingHeight,
defaultFramebuffer = _defaultFramebuffer,
colorRenderbuffer = _colorRenderbuffer,
depthRenderbuffer = _depthRenderbuffer
;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

-(id)initWithFrame:(CGRect)fr
{
  DOUT("");
  if(self = [super initWithFrame:fr])
  {
    // initialise basic layer properties
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;        
    // FIXME: these should probably be configurable from TaskletConfig
    eaglLayer.opaque = TRUE;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
    // create context
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];        
    if (!_context || ![EAGLContext setCurrentContext:_context])
		{
      EOUT("failed to create context, aborting");
      [self release];
      return nil;
    }
    
    // configure framebuffer
    // FIXME: this needs to be configurable via TaskletConfig, has only colorbuffer for a start
		glGenFramebuffers(1, &_defaultFramebuffer);GLDEBUG;
		glGenRenderbuffers(1, &_colorRenderbuffer);GLDEBUG;
    glGenRenderbuffers(1, &_depthRenderbuffer);GLDEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);GLDEBUG;
		glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);GLDEBUG;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);GLDEBUG;
    
    
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);GLDEBUG;
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screenRect.size.width, screenRect.size.height);GLDEBUG;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);GLDEBUG;
    
//		glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);GLDEBUG;
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);GLDEBUG;
  }
  return self;
}

- (void) layoutSubviews
{
  DOUT("");
  glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);GLDEBUG;
  CAEAGLLayer* eaglLayer = (CAEAGLLayer *)self.layer;        
  [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);GLDEBUG;
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);GLDEBUG;
  DOUT(_backingWidth << ", " << _backingHeight);

/*  glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _backingWidth, _backingHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderbuffer);*/
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
      EOUT("Failed to make complete framebuffer object " << glCheckFramebufferStatus(GL_FRAMEBUFFER));
  }  
}

-(void)setParentWindow:(lost::application::Window *)parent
{
  _parent = parent;
}

- (void)touchEvent: (NSSet*)touches type:(lost::event::Type)type pressed:(bool)pressed
{
  if (_parent)
  {
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    lost::shared_ptr<lost::application::MouseEvent> mouseEvent(new lost::application::MouseEvent(type));
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView: self];
    mouseEvent->window  = _parent;
    mouseEvent->pos     = lost::math::Vec2(touchLocation.x, screenRect.size.height - touchLocation.y);
    mouseEvent->absPos  = lost::math::Vec2(touchLocation.x, screenRect.size.height - touchLocation.y);
    mouseEvent->button  = lost::application::MB_LEFT;
    mouseEvent->pressed = pressed;
    mouseEvent->scrollDelta = lost::math::Vec3(0, 0, 0);
    _parent->dispatcher->queueEvent(mouseEvent);
  }  
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self touchEvent: touches type: lost::application::MouseEvent::MOUSE_DOWN() pressed: YES];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self touchEvent: touches type: lost::application::MouseEvent::MOUSE_MOVE() pressed: YES];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self touchEvent: touches type: lost::application::MouseEvent::MOUSE_UP() pressed: NO];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self touchEvent: touches type: lost::application::MouseEvent::MOUSE_UP() pressed: NO];
}

@end

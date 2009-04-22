#include "lost/application/Window.h"

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import "lost/gl/iphone/GLContext.h"

#include "lost/common/Logger.h"
#include "lost/application/TouchEvent.h"

@interface ApplicationWindow : UIWindow
{
}
@end

@implementation ApplicationWindow
@end

@interface GLView : UIView
{
  /* The pixel dimensions of the backbuffer */
  GLint backingWidth;
  GLint backingHeight;
  
  GLContext* openGLContext;
  
  lost::application::Window* parent;

  std::list<lost::shared_ptr<lost::application::TouchEvent::Touch> >* touches;
  NSUInteger maxNumTouches;
}

@end

@implementation GLView

- (GLContext*)openGLContext
{
  return openGLContext;
}

- (void)setParent: (lost::application::Window*)newParent
{
  parent = newParent;
}

// You must implement this method
+ (Class)layerClass
{
  return [CAEAGLLayer class];
}

- (id)initWithFrame: (CGRect)frame
{
  if ((self = [super initWithFrame:frame]))
  {
    maxNumTouches = 10;

    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
    openGLContext = [[GLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
    if (!openGLContext || ![EAGLContext setCurrentContext:openGLContext])
    {
      [self release];
      return nil;
    }
  }
  return self;
}

- (BOOL)createFramebuffer
{
  glGenFramebuffersOES(1, &openGLContext->viewFramebuffer);
  glGenRenderbuffersOES(1, &openGLContext->viewRenderbuffer);
  
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, openGLContext->viewFramebuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, openGLContext->viewRenderbuffer);
  [openGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, openGLContext->viewRenderbuffer);
  
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
  
  glGenRenderbuffersOES(1, &openGLContext->depthRenderbuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, openGLContext->depthRenderbuffer);
  glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, openGLContext->depthRenderbuffer);
  
  if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
  {
    NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    return NO;
  }
  
  return YES;
}

- (void)destroyFramebuffer
{
  glDeleteFramebuffersOES(1, &openGLContext->viewFramebuffer);
  openGLContext->viewFramebuffer = 0;
  glDeleteRenderbuffersOES(1, &openGLContext->viewRenderbuffer);
  openGLContext->viewRenderbuffer = 0;
  glDeleteRenderbuffersOES(1, &openGLContext->depthRenderbuffer);
  openGLContext->depthRenderbuffer = 0;
}

- (void)layoutSubviews
{
  [EAGLContext setCurrentContext:openGLContext];
  [self destroyFramebuffer];
  [self createFramebuffer];
}

- (void)dealloc
{
  if ([EAGLContext currentContext] == openGLContext)
  {
    [EAGLContext setCurrentContext:nil];
  }
  
  [openGLContext release];  
  [super dealloc];
}

-(void) convertTouches: (UIEvent*)event type: (lost::event::Type)type
{
  if (parent)
  {
    lost::shared_ptr<lost::application::TouchEvent> touchEvent(new lost::application::TouchEvent(type));
    unsigned int count = 0;
    for(UITouch* touch in [event allTouches])
    {    
      lost::shared_ptr<lost::application::TouchEvent::Touch> newTouch(new lost::application::TouchEvent::Touch);
      newTouch->tapCount = touch.tapCount;
      newTouch->timeStamp = touch.timestamp;
      CGPoint loc = [touch locationInView:self];
      newTouch->location = lost::math::Vec2(loc.x, backingHeight - loc.y);
      touchEvent->touches.push_back(newTouch);
      if (count++ >= maxNumTouches)
      {
        EOUT("too many touches, dropping remaining");
        break;
      }
    }
    parent->dispatcher->queueEvent(touchEvent);
  }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self convertTouches:event type: lost::application::TouchEvent::TOUCHES_BEGAN()];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self convertTouches:event type: lost::application::TouchEvent::TOUCHES_MOVED()];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self convertTouches:event type: lost::application::TouchEvent::TOUCHES_ENDED()];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
  [self convertTouches:event type: lost::application::TouchEvent::TOUCHES_CANCELLED()];
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
      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      // create window
      hiddenMembers->window = [[ApplicationWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];

      // set params
//      hiddenMembers->window.backgroundColor = [UIColor whiteColor];  
//      open();

      // create view
      hiddenMembers->view = [[GLView alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
      [hiddenMembers->view setParent: this];
      [hiddenMembers->window addSubview: hiddenMembers->view];

      [EAGLContext setCurrentContext: [hiddenMembers->view openGLContext]];
      context.reset(new gl::Context);
    }

    void Window::finalize()
    {
      [hiddenMembers->view release];
      [hiddenMembers->window release];
      delete hiddenMembers;
    }

    void Window::open()
    {
      [hiddenMembers->window makeKeyAndVisible];
    }
    
    void Window::close()
    {
      // TODO: implement close (if this makes sense at all)
    }

  }
}

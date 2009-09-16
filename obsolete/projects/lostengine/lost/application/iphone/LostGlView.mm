#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "lost/application/iphone/LostGlView.h"
#import "lost/common/Logger.h"
#include "lost/gl/Utils.h"
#include "lost/application/iphone/LostApplicationHelpers.h"
#include <boost/shared_ptr.hpp>
#include "lost/math/Vec2.h"

using namespace boost;
using namespace lost;
using namespace lost::math;
using namespace lost::application;

@implementation LostGlView

//@synthesize animationInterval;

// Implement this to override the default layer class (which is [CALayer class]).
// We do this so that our view will be backed by a layer that is capable of OpenGL ES rendering.
+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

// When created via code however, we get initWithFrame
-(id)initWithFrame:(CGRect)frame
{
	self = [super initWithFrame:frame];
	if(self != nil)
	{
		self = [self initGLES];
	}
	return self;
}

//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{
	if((self = [super initWithCoder:coder]))
	{
		self = [self initGLES];
	}	
	return self;
}

-(id)initGLES
{
  // FIXME: clean up init process, it makes no sense to init general members in initGLES
  touchEvent = new shared_ptr<TouchEvent>;
  touchEvent->reset(new TouchEvent(""));
  
  touches = new std::list<boost::shared_ptr<lost::application::TouchEvent::Touch> >;
  maxNumTouches = 10;
  for(NSUInteger i=0; i<maxNumTouches; ++i)
  {
    touches->push_back(shared_ptr<TouchEvent::Touch>(new TouchEvent::Touch));
  }
  
	// Get our backing layer
	CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;
	
	// Configure it so that it is opaque, does not retain the contents of the backbuffer when displayed, and uses RGBA8888 color.
	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
										kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
										nil];
	
	// Create our EAGLContext, and if successful make it current and create our framebuffer.
	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	if(!context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer])
	{
		[self release];
		return nil;
	}
	
	return self;
}

-(id<LostGlViewDelegate>)delegate
{
	return delegate;
}

// Update the delegate, and if it needs a -setupView: call, set our internal flag so that it will be called.
-(void)setDelegate:(id<LostGlViewDelegate>)d
{
	delegate = d;
}

// If our view is resized, we'll be asked to layout subviews.
// This is the perfect opportunity to also update the framebuffer so that it is
// the same size as our display area.
-(void)layoutSubviews
{
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
	[self drawView];
}

- (BOOL)createFramebuffer
{
	// Generate IDs for a framebuffer object and a color renderbuffer
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	// This call associates the storage for the current render buffer with the EAGLDrawable (our CAEAGLLayer)
	// allowing us to draw into a buffer that will later be rendered to screen whereever the layer is (which corresponds with our view).
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	// For this sample, we also need a depth buffer, so we'll create and attach one via another renderbuffer.
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);

	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		DOUT("failed to make complete framebuffer object "<< glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}
	
	return YES;
}

// Clean up any buffers we have allocated.
- (void)destroyFramebuffer
{
	glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
	
	if(depthRenderbuffer)
	{
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
}

// Updates the OpenGL view when the timer fires
- (void)drawView
{
	// Make sure that you are drawing to the current context
	[EAGLContext setCurrentContext:context];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	[delegate drawView:self];
  [self swapBuffers];
  GLDEBUG;
}

- (void)swapBuffers
{
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


// Stop animating and release resources when they are no longer needed.
- (void)dealloc
{	
  if(touchEvent) { delete touchEvent; } 
  if(touches) { delete touches; }
	if([EAGLContext currentContext] == context)
	{
		[EAGLContext setCurrentContext:nil];
	}
	
	[context release];
	context = nil;
	
	[super dealloc];
}

-(void) convertTouches:(UIEvent*)event
{
  if([[event allTouches] count] > maxNumTouches)
  {
    EOUT("too many touches, dropping all");
    return;
  }
  
  (*touchEvent)->touches.clear();
  std::list<boost::shared_ptr<lost::application::TouchEvent::Touch> >::iterator i = touches->begin();
  for(UITouch* touch in [event allTouches])
  {
    (*i)->tapCount = touch.tapCount;
    (*i)->timeStamp = touch.timestamp;
    CGPoint loc = [touch locationInView:self];
    (*i)->location = Vec2(loc.x, backingHeight - loc.y);
    (*touchEvent)->touches.push_back(*i);
  }
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  (*touchEvent)->type = TouchEvent::TOUCHES_BEGAN();
  [self convertTouches:event];
  lostApplicationHelpers_dispatchEvent(*touchEvent);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  (*touchEvent)->type = TouchEvent::TOUCHES_MOVED();
  [self convertTouches:event];
  lostApplicationHelpers_dispatchEvent(*touchEvent);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  (*touchEvent)->type = TouchEvent::TOUCHES_ENDED();
  [self convertTouches:event];
  lostApplicationHelpers_dispatchEvent(*touchEvent);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  (*touchEvent)->type = TouchEvent::TOUCHES_CANCELLED();
  [self convertTouches:event];
  lostApplicationHelpers_dispatchEvent(*touchEvent);
}

@end

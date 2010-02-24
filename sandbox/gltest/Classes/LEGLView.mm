#import "LEGLView.h"
#import <QuartzCore/CAEAGLLayer.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@implementation LEGLView

@synthesize
context = _context,
backingWidth = _backingWidth,
backingHeight = _backingHeight,
defaultFramebuffer = _defaultFramebuffer,
colorRenderbuffer = _colorRenderbuffer
;

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

-(id)initWithFrame:(CGRect)fr
{
  NSLog(@"%s", __FUNCTION__);
  if(self = [super initWithFrame:fr])
  {
    // initialise basic layer properties
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;        
    // FIXME: these should probably be configurable from WindowParams
    eaglLayer.opaque = TRUE;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                    kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
        
    // create context
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];        
    if (!_context || ![EAGLContext setCurrentContext:_context])
		{
      [self release];
      return nil;
    }
    
    // configure framebuffer
    // FIXME: this needs to be configurable via WindowParams, has only colorbuffer for a start
		glGenFramebuffers(1, &_defaultFramebuffer);GLDEBUG;
		glGenRenderbuffers(1, &_colorRenderbuffer);GLDEBUG;
		glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);GLDEBUG;
		glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);GLDEBUG;
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);GLDEBUG;
  }
  return self;
}

- (void) layoutSubviews
{
  NSLog(@"%s", __FUNCTION__);
  glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);GLDEBUG;
  CAEAGLLayer* eaglLayer = (CAEAGLLayer *)self.layer;        
  [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &_backingWidth);GLDEBUG;
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &_backingHeight);GLDEBUG;

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
      NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
  }  
  GLDEBUG;
}

@end
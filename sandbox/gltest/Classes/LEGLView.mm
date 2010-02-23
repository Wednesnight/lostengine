#import "LEGLView.h"
#import <QuartzCore/CAEAGLLayer.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@implementation LEGLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

-(id)initWithFrame:(CGRect)fr andApi:(EAGLRenderingAPI)api;
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
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];        
    if (!_context || ![EAGLContext setCurrentContext:_context])
		{
      [self release];
      return nil;
    }
    
    // configure framebuffer
    // FIXME: this needs to be configurable via WindowParams, has only colorbuffer for a start
		glGenFramebuffers(1, &_defaultFramebuffer);
		glGenRenderbuffers(1, &_colorRenderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _defaultFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderbuffer);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderbuffer);    
  }
  return self;
}

- (void) layoutSubviews
{
  // FIXME: resize framebuffer here
  // FIXME: force render after rsize? it's done in the GL example
}

@end
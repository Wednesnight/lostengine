#import "LEGLView.h"
#import <QuartzCore/CAEAGLLayer.h>

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
		glGenFramebuffersOES(1, &defaultFramebuffer);
		glGenRenderbuffersOES(1, &colorRenderbuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);    
  }
  return self;
}

- (void) layoutSubviews
{
  // FIXME: resize framebuffer here
  // FIXME: force render after rsize? it's done in the GL example
}

@end
#include "lost/application/Window.h"

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>

#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/application/KeyCode.h"

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
  
  EAGLContext* openGLContext;
  
  /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
  GLuint viewRenderbuffer, viewFramebuffer;
  
  /* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
  GLuint depthRenderbuffer;

  lost::application::Window* parent;
}

@end

@implementation GLView

- (EAGLContext*)openGLContext
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
    // Get the layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
    openGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
    
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
  glGenFramebuffersOES(1, &viewFramebuffer);
  glGenRenderbuffersOES(1, &viewRenderbuffer);
  
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [openGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
  
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
  glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
  
  glGenRenderbuffersOES(1, &depthRenderbuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
  glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
  glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
  
  if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
  {
    NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
    return NO;
  }
  
  return YES;
}

- (void)destroyFramebuffer
{
  glDeleteFramebuffersOES(1, &viewFramebuffer);
  viewFramebuffer = 0;
  glDeleteRenderbuffersOES(1, &viewRenderbuffer);
  viewRenderbuffer = 0;
  glDeleteRenderbuffersOES(1, &depthRenderbuffer);
  depthRenderbuffer = 0;
}

- (void)drawView
{
	// Make sure that you are drawing to the current context
	[EAGLContext setCurrentContext: openGLContext];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  if (parent) parent->context->swapBuffers();
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
      DOUT("Window::finalize()");
      // FIXME: cleanup!
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

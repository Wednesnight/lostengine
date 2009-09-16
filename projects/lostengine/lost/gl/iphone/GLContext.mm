#import "lost/gl/iphone/GLContext.h"

@implementation GLContext

- (void)swapBuffersOnMainThread
{
  [EAGLContext setCurrentContext: self];
  glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
  glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
  [self presentRenderbuffer: GL_RENDERBUFFER_OES];
}

- (void)swapBuffers
{
  [self performSelectorOnMainThread: @selector(swapBuffersOnMainThread) withObject: nil waitUntilDone: YES];
}

@end

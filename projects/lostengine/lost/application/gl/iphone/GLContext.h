#ifndef LOST_APPLICATION_GL_IPHONE_CONTEXT_H
#define LOST_APPLICATION_GL_IPHONE_CONTEXT_H

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

/**
 * our own implementation for the GL context, buffers are setup within View creation
 */
@interface GLContext : EAGLContext
{
@public
  /* OpenGL names for the renderbuffer, framebuffer and depthbuffer used to render to the view */
  GLuint viewRenderbuffer, viewFramebuffer, depthRenderbuffer;
}
@end

#endif

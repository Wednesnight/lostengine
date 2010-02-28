#ifndef LOST_LGL_LGL_GLES_H
#define LOST_LGL_LGL_GLES_H

#include "lost/gl/gl.h"

#define LGL_RENDERBUFFER                 GL_RENDERBUFFER_OES
#define LGL_FRAMEBUFFER                  GL_FRAMEBUFFER_OES
#define LGL_RENDERBUFFER_WIDTH           GL_RENDERBUFFER_WIDTH_OES
#define LGL_RENDERBUFFER_HEIGHT          GL_RENDERBUFFER_HEIGHT_OES
#define LGL_RENDERBUFFER_INTERNAL_FORMAT GL_RENDERBUFFER_INTERNAL_FORMAT_OES
#define LGL_RENDERBUFFER_RED_SIZE        GL_RENDERBUFFER_RED_SIZE_OES
#define LGL_RENDERBUFFER_GREEN_SIZE      GL_RENDERBUFFER_GREEN_SIZE_OES
#define LGL_RENDERBUFFER_BLUE_SIZE       GL_RENDERBUFFER_BLUE_SIZE_OES
#define LGL_RENDERBUFFER_ALPHA_SIZE      GL_RENDERBUFFER_ALPHA_SIZE_OES
#define LGL_RENDERBUFFER_DEPTH_SIZE      GL_RENDERBUFFER_DEPTH_SIZE_OES
#define LGL_RENDERBUFFER_STENCIL_SIZE    GL_RENDERBUFFER_STENCIL_SIZE_OES
#define LGL_COLOR_ATTACHMENT0            GL_COLOR_ATTACHMENT0_OES
#define LGL_STENCIL_ATTACHMENT           GL_STENCIL_ATTACHMENT_OES
#define LGL_DEPTH_ATTACHMENT             GL_DEPTH_ATTACHMENT_OES
#define LGL_DEPTH_COMPONENT              GL_DEPTH_COMPONENT16_OES
#define LGL_DEPTH_COMPONENT16            GL_DEPTH_COMPONENT16_OES
#define LGL_DEPTH_COMPONENT24            GL_DEPTH_COMPONENT24_OES
#define LGL_FRAMEBUFFER_COMPLETE         GL_FRAMEBUFFER_COMPLETE_OES

#define LGL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT   GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
#define LGL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT   GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
#define LGL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS   GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
#define LGL_FRAMEBUFFER_INCOMPLETE_FORMATS   GL_FRAMEBUFFER_INCOMPLETE_FORMATS_OES
#define LGL_FRAMEBUFFER_UNSUPPORTED   GL_FRAMEBUFFER_UNSUPPORTED_OES

#define LGL_FRAMEBUFFER_BINDING          GL_FRAMEBUFFER_BINDING_OES

static inline void lglOrtho(GLfloat left,
              GLfloat right,
              GLfloat bottom,
              GLfloat top,
              GLfloat near,
              GLfloat far)
{
  glOrthof(left,
           right,
           bottom,
           top,
           near,
           far);
}

static inline void lglGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
  glGenRenderbuffersOES(n, renderbuffers);
}

static inline void lglBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
  glBindRenderbufferOES(target, renderbuffer);
}

static inline void lglBindFramebuffer(GLenum target, GLuint framebuffer)
{
  glBindFramebufferOES(target, framebuffer);
}

static inline void lglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  glRenderbufferStorageOES(target, internalformat, width, height);
}

static inline void lglGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
  glGetRenderbufferParameterivOES(target, pname, params);
}

static inline void lglDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
  glDeleteRenderbuffersOES(n, renderbuffers);
}

static inline void lglDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
  glDeleteFramebuffersOES(n, framebuffers);
}

static inline void lglGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
  glGenFramebuffersOES(n, framebuffers);
}

static inline void lglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  glFramebufferRenderbufferOES(target, attachment, renderbuffertarget, renderbuffer);
}

static inline void lglFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  glFramebufferTexture2DOES(target, attachment, textarget, texture, level);
}

static inline GLenum lglCheckFramebufferStatus(GLenum target)
{
  return glCheckFramebufferStatusOES(target);
}

#endif

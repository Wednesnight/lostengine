#ifndef LOST_LGL_LGL_GL_H
#define LOST_LGL_LGL_GL_H

#include "lost/gl/gl.h"

#define LGL_RENDERBUFFER                 GL_RENDERBUFFER_EXT
#define LGL_FRAMEBUFFER                  GL_FRAMEBUFFER_EXT
#define LGL_RENDERBUFFER_WIDTH           GL_RENDERBUFFER_WIDTH_EXT
#define LGL_RENDERBUFFER_HEIGHT          GL_RENDERBUFFER_HEIGHT_EXT
#define LGL_RENDERBUFFER_INTERNAL_FORMAT GL_RENDERBUFFER_INTERNAL_FORMAT_EXT
#define LGL_RENDERBUFFER_RED_SIZE        GL_RENDERBUFFER_RED_SIZE_EXT
#define LGL_RENDERBUFFER_GREEN_SIZE      GL_RENDERBUFFER_GREEN_SIZE_EXT
#define LGL_RENDERBUFFER_BLUE_SIZE       GL_RENDERBUFFER_BLUE_SIZE_EXT
#define LGL_RENDERBUFFER_ALPHA_SIZE      GL_RENDERBUFFER_ALPHA_SIZE_EXT
#define LGL_RENDERBUFFER_DEPTH_SIZE      GL_RENDERBUFFER_DEPTH_SIZE_EXT
#define LGL_RENDERBUFFER_STENCIL_SIZE    GL_RENDERBUFFER_STENCIL_SIZE_EXT
#define LGL_COLOR_ATTACHMENT0            GL_COLOR_ATTACHMENT0_EXT
#define LGL_STENCIL_ATTACHMENT           GL_STENCIL_ATTACHMENT_EXT
#define LGL_DEPTH_ATTACHMENT             GL_DEPTH_ATTACHMENT_EXT
#define LGL_DEPTH_COMPONENT16            GL_DEPTH_COMPONENT16
#define LGL_DEPTH_COMPONENT24            GL_DEPTH_COMPONENT24
#define LGL_FRAMEBUFFER_COMPLETE         GL_FRAMEBUFFER_COMPLETE_EXT
#define LGL_FRAMEBUFFER_BINDING          GL_FRAMEBUFFER_BINDING_EXT

static inline void lglOrtho(GLfloat left,
              GLfloat right,
              GLfloat bottom,
              GLfloat top,
              GLfloat znear,
              GLfloat zfar)
{
  glOrtho(static_cast<GLdouble>(left),
          static_cast<GLdouble>(right),
          static_cast<GLdouble>(bottom),
          static_cast<GLdouble>(top),
          static_cast<GLdouble>(znear),
          static_cast<GLdouble>(zfar));
}

static inline void lglGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
  glGenRenderbuffersEXT(n, renderbuffers);
}

static inline void lglBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
  glBindRenderbufferEXT(target, renderbuffer);
}

static inline void lglBindFramebuffer(GLenum target, GLuint framebuffer)
{
  glBindFramebufferEXT(target, framebuffer);
}

static inline void lglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  glRenderbufferStorageEXT(target, internalformat, width, height);
}

static inline void lglGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
  glGetRenderbufferParameterivEXT(target, pname, params);
}

static inline void lglDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
  glDeleteRenderbuffersEXT(n, renderbuffers);
}

static inline void lglDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
  glDeleteFramebuffersEXT(n, framebuffers);
}

static inline void lglGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
  glGenFramebuffersEXT(n, framebuffers);
}

static inline void lglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  glFramebufferRenderbufferEXT(target, attachment, renderbuffertarget, renderbuffer);
}

static inline void lglFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  glFramebufferTexture2DEXT(target, attachment, textarget, texture, level);
}

static inline GLenum lglCheckFramebufferStatus(GLenum target)
{
  return glCheckFramebufferStatusEXT(target);
}

#endif

/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_LGL_LGL_GLES_H
#define LOST_LGL_LGL_GLES_H

#include "lost/gl/gl.h"

#define LGL_RENDERBUFFER                 GL_RENDERBUFFER
#define LGL_FRAMEBUFFER                  GL_FRAMEBUFFER
#define LGL_RENDERBUFFER_WIDTH           GL_RENDERBUFFER_WIDTH
#define LGL_RENDERBUFFER_HEIGHT          GL_RENDERBUFFER_HEIGHT
#define LGL_RENDERBUFFER_INTERNAL_FORMAT GL_RENDERBUFFER_INTERNAL_FORMAT
#define LGL_RENDERBUFFER_RED_SIZE        GL_RENDERBUFFER_RED_SIZE
#define LGL_RENDERBUFFER_GREEN_SIZE      GL_RENDERBUFFER_GREEN_SIZE
#define LGL_RENDERBUFFER_BLUE_SIZE       GL_RENDERBUFFER_BLUE_SIZE
#define LGL_RENDERBUFFER_ALPHA_SIZE      GL_RENDERBUFFER_ALPHA_SIZE
#define LGL_RENDERBUFFER_DEPTH_SIZE      GL_RENDERBUFFER_DEPTH_SIZE
#define LGL_RENDERBUFFER_STENCIL_SIZE    GL_RENDERBUFFER_STENCIL_SIZE
#define LGL_COLOR_ATTACHMENT0            GL_COLOR_ATTACHMENT0
#define LGL_STENCIL_ATTACHMENT           GL_STENCIL_ATTACHMENT
#define LGL_DEPTH_ATTACHMENT             GL_DEPTH_ATTACHMENT
#define LGL_DEPTH_COMPONENT              GL_DEPTH_COMPONENT16
#define LGL_DEPTH_COMPONENT16            GL_DEPTH_COMPONENT16
#define LGL_DEPTH_COMPONENT24            GL_DEPTH_COMPONENT24_OES
#define LGL_FRAMEBUFFER_COMPLETE         GL_FRAMEBUFFER_COMPLETE

#define LGL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT   GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
#define LGL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT   GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
#define LGL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS   GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
#define LGL_FRAMEBUFFER_INCOMPLETE_FORMATS   GL_FRAMEBUFFER_INCOMPLETE_FORMATS
#define LGL_FRAMEBUFFER_UNSUPPORTED   GL_FRAMEBUFFER_UNSUPPORTED

#define LGL_FRAMEBUFFER_BINDING          GL_FRAMEBUFFER_BINDING

static inline void lglGenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
  glGenRenderbuffers(n, renderbuffers);
}

static inline void lglBindRenderbuffer(GLenum target, GLuint renderbuffer)
{
  glBindRenderbuffer(target, renderbuffer);
}

static inline void lglBindFramebuffer(GLenum target, GLuint framebuffer)
{
  glBindFramebuffer(target, framebuffer);
}

static inline void lglRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
  glRenderbufferStorage(target, internalformat, width, height);
}

static inline void lglGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
  glGetRenderbufferParameteriv(target, pname, params);
}

static inline void lglDeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
  glDeleteRenderbuffers(n, renderbuffers);
}

static inline void lglDeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
  glDeleteFramebuffers(n, framebuffers);
}

static inline void lglGenFramebuffers(GLsizei n, GLuint* framebuffers)
{
  glGenFramebuffers(n, framebuffers);
}

static inline void lglFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
  glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

static inline void lglFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
  glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

static inline GLenum lglCheckFramebufferStatus(GLenum target)
{
  return glCheckFramebufferStatus(target);
}

#endif

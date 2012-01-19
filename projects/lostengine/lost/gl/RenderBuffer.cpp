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

#include "lost/gl/RenderBuffer.h"

#include "lost/lgl/lgl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{

RenderBuffer::RenderBuffer()
{
  lglGenRenderbuffers(1, &buffer);GLDEBUG_THROW;
}

GLint RenderBuffer::width() { return param(LGL_RENDERBUFFER_WIDTH); }
GLint RenderBuffer::height() { return param(LGL_RENDERBUFFER_HEIGHT); }
GLint RenderBuffer::internalFormat() { return param(LGL_RENDERBUFFER_INTERNAL_FORMAT); }
GLint RenderBuffer::redSize() { return param(LGL_RENDERBUFFER_RED_SIZE); }
GLint RenderBuffer::greenSize() { return param(LGL_RENDERBUFFER_GREEN_SIZE); }
GLint RenderBuffer::blueSize() { return param(LGL_RENDERBUFFER_BLUE_SIZE); }
GLint RenderBuffer::alphaSize() { return param(LGL_RENDERBUFFER_ALPHA_SIZE); }
GLint RenderBuffer::depthSize() { return param(LGL_RENDERBUFFER_DEPTH_SIZE); }
GLint RenderBuffer::stencilSize() { return param(LGL_RENDERBUFFER_STENCIL_SIZE); }

void RenderBuffer::enable()
{
  lglBindRenderbuffer(LGL_RENDERBUFFER, buffer);GLDEBUG_THROW;
}

void RenderBuffer::disable()
{
  lglBindRenderbuffer(LGL_RENDERBUFFER, 0);GLDEBUG_THROW;
}

void RenderBuffer::storage(GLenum inInternalFormat, GLsizei inWidth, GLsizei inHeight)
{
  lglRenderbufferStorage(LGL_RENDERBUFFER, inInternalFormat, inWidth, inHeight);GLDEBUG_THROW;
}

GLint RenderBuffer::param(GLenum paramName)
{
  GLint result;
  lglGetRenderbufferParameteriv(buffer, paramName, &result);GLDEBUG_THROW;
  return result;
}

RenderBuffer::~RenderBuffer()
{
  lglDeleteRenderbuffers(1, &buffer);GLDEBUG;
}

}
}

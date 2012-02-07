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

#ifndef LOST_GL_HYBRIDINDEXBUFFER_H
#define LOST_GL_HYBRIDINDEXBUFFER_H

#include "lost/gl/HybridBuffer.h"
#include "lost/gl/forward.h"

namespace lost
{
namespace gl
{

struct HybridIndexBuffer : HybridBuffer
{
  // since an IndexBuffer is only an array of integers, we can simplify the 
  // layouting of the buffer by only requiring a single element type parameter.
  HybridIndexBuffer(ElementType et);
  ~HybridIndexBuffer();
  
  GLenum type; // required for the actual draw operation, derived in constructor
  GLenum drawMode; // GL_LINES, GL_TRIANGLES etc.
};

}
}

#endif

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

#include "lost/gl/HybridIndexBuffer.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include "lost/profiler/Blackbox.h"

namespace lost
{
namespace gl
{

#define bb_hib_key "lost.gl.HybridIndexBuffer"

HybridIndexBuffer::HybridIndexBuffer(ElementType et)
{
  // an indexbuffer only ever has one attribute with usage type index in a single partition
  // only the element type can vary, to optimize the buffer for hardware requirements or
  // mesh sizes.
  BufferLayout layout;
  layout.add(et, UT_index);
  switch(et)
  {
    case ET_u8:type = GL_UNSIGNED_BYTE;break;
    case ET_u16:type = GL_UNSIGNED_SHORT;break;
    case ET_u32:type = GL_UNSIGNED_INT;break;
    default:
      lost::common::StringStream os;
      os << "only u8, u16, u32 are allowed";
      LOGTHROW(std::runtime_error(os.str().c_str()));
  }
  drawMode = GL_TRIANGLES;
  init(GL_ELEMENT_ARRAY_BUFFER, layout);
  BB_INC(bb_hib_key);
}

HybridIndexBuffer::~HybridIndexBuffer()
{
  BB_DEC(bb_hib_key);
}

}
}

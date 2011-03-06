#include "lost/gl/HybridIndexBuffer.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include "lost/profiler/Blackbox.h"

namespace lost
{
namespace gl
{

using namespace std;

#define bb_hib_key "lost.gl.HybridIndexBuffer"

HybridIndexBuffer::HybridIndexBuffer(ElementType et)
{
  // an indexbuffer only ever has one attribute with usage type index in a single partition
  // only the element type can vary, to optimize the buffer for hardware requirements or
  // mesh sizes.
  BufferLayout layout;
  layout.add(et, UT_index, 0);
  switch(et)
  {
    case ET_u8:type = GL_UNSIGNED_BYTE;break;
    case ET_u16:type = GL_UNSIGNED_SHORT;break;
    case ET_u32:type = GL_UNSIGNED_INT;break;
    default:
      ostringstream os;
      os << "only u8, u16, u32 are allowed";
      LOGTHROW(runtime_error(os.str()));
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

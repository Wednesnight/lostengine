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
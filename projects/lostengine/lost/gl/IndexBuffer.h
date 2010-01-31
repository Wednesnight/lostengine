#ifndef LOST_GL_INDEXBUFFER_H
#define LOST_GL_INDEXBUFFER_H

#include "lost/gl/Buffer.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
namespace gl
{

struct IndexBuffer;
typedef lost::shared_ptr<IndexBuffer> IndexBufferPtr;

struct IndexBuffer : Buffer
{
  IndexBuffer()
  {
    target = GL_ELEMENT_ARRAY_BUFFER;
  }
  
  virtual ~IndexBuffer() {}
  
};

}
}

#endif

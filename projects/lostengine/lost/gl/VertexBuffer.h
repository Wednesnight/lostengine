#ifndef LOST_GL_VERTEXBUFFER_H
#define LOST_GL_VERTEXBUFFER_H

#include "lost/gl/Buffer.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
namespace gl
{

struct VertexBuffer;
typedef lost::shared_ptr<VertexBuffer> VertexBufferPtr;

struct VertexBuffer : Buffer
{
  VertexBuffer()
  {
    target = GL_ARRAY_BUFFER;
  }
  
  virtual ~VertexBuffer() {}
  
};

}
}

#endif

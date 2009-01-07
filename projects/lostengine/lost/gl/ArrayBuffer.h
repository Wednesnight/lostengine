#ifndef LOST_GL_ARRAYBUFFER_H
#define LOST_GL_ARRAYBUFFER_H

#include "lost/gl/TypedBuffer.h"

namespace lost
{
namespace gl
{
template<typename T>
struct ArrayBuffer : public TypedBuffer<T>
{
public:

  ArrayBuffer()
  {
    TypedBuffer<T>::target = GL_ARRAY_BUFFER;
  }

  virtual ~ArrayBuffer()
  {
  }

};
}
}

#endif

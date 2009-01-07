#ifndef LOST_GL_ELEMENTARRAYBUFFER_H
#define LOST_GL_ELEMENTARRAYBUFFER_H

#include "lost/gl/TypedBuffer.h"

namespace lost
{
namespace gl
{

// for element arrays, type must be one of these
// GL_UNSIGNED_BYTE
// GL_UNSIGNED_SHORT
// GL_UNSIGNED_INT
template<typename T>
struct ElementArrayBuffer : public TypedBuffer<T>
{
  ElementArrayBuffer()
  {
    TypedBuffer<T>::target=GL_ELEMENT_ARRAY_BUFFER;
  }

  virtual ~ElementArrayBuffer()
  {
  }
};
}
}

#endif

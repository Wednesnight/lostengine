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
// these are the common use cases, also handy for lua bindings.
typedef ElementArrayBuffer<uint8_t>  ElementArrayBufferU8;
typedef ElementArrayBuffer<uint16_t> ElementArrayBufferU16;
typedef ElementArrayBuffer<uint32_t> ElementArrayBufferU32;

}
}

#endif

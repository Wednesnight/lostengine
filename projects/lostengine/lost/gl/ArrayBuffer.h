#ifndef LOST_GL_ARRAYBUFFER_H
#define LOST_GL_ARRAYBUFFER_H

#include "lost/gl/TypedBuffer.h"

namespace lost
{
namespace gl
{

/** use ArrayBuffers to build  etc. buffer objects.
 * the type T is your vertex/normal/color/texcoord type.
 * In order for this to work, the traits type T must be registered
 * in TraitsSelectors.h
 */
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

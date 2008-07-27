#ifndef LOST_GL_ARRAYBUFFER_H
#define LOST_GL_ARRAYBUFFER_H

#include "lost/gl/Buffer.h"

namespace lost
{
namespace gl
{
template<typename T>
struct ArrayBuffer : public Buffer<T>
{
public:
  
  ArrayBuffer()
  {  
    Buffer<T>::target = GL_ARRAY_BUFFER;
  }
  
  virtual ~ArrayBuffer()
  {
  }
  
};
}
}

#endif
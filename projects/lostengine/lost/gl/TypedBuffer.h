#ifndef LOST_GL_TYPEDBUFFER_H
#define LOST_GL_TYPEDBUFFER_H

#include "lost/gl/TraitsSelector.h"
#include "lost/gl/Buffer.h"

namespace lost
{
  namespace gl
  {

    template<typename T>
    struct TypedBuffer : public Buffer
    {
    public:
      TypedBuffer()
      {
        TraitsSelector<T> traits;
        size = traits.size;
        type = traits.type;
      }

      virtual ~TypedBuffer()
      {
      }

      virtual void bindBufferData(T* inData, uint32_t numElems, GLenum usage=GL_STATIC_DRAW)
      {
        Buffer::bindBufferData(sizeof(T)*numElems, inData, usage);
        numElements = numElems;
      }

    };

  }
}

#endif

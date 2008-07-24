#ifndef LOST_GL_ARRAYBUFFER_H
#define LOST_GL_ARRAYBUFFER_H

#include "lost/gl/Buffer.h"
#include "lost/gl/TraitsSelector.h"

namespace lost
{
namespace gl
{
template<typename T>
struct ArrayBuffer : public Buffer
{
public:
  GLenum target;
  GLint size;
  GLenum type;
  
  unsigned long numElements; // number of verts/colors/texcoords in array
  
  ArrayBuffer()
  {
    TraitsSelector<T> traits;
    size = traits.size;
    type = traits.type;
  
    target = GL_ARRAY_BUFFER;
    numElements = 0;
  }
  
  virtual ~ArrayBuffer()
  {
  }
  
  virtual void bind() { Buffer::bind(target); }
  virtual void bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage=GL_STATIC_DRAW) {Buffer::bufferData(target, size, data, usage);}
  virtual void bindBufferData(GLsizeiptr size, const GLvoid* data, GLenum usage=GL_STATIC_DRAW) {bind();Buffer::bufferData(target, size, data, usage);}
  virtual void bindBufferData(T* inData, unsigned long numElems, GLenum usage=GL_STATIC_DRAW)
  {
    GLsizeiptr sizeInBytes = sizeof(T)*numElems;
    bindBufferData(sizeInBytes, inData, usage);
    numElements = numElems;
  }
  
  
  void bindVertexPointer(GLsizei stride=0, const GLvoid* pointer=0){bind();vertexPointer(size, type, stride, pointer);}
  void bindTexCoordPointer(GLsizei stride=0, const GLvoid* pointer=0) {bind();texCoordPointer(size, type, stride, pointer);}
  void bindNormalPointer(GLsizei stride=0, const GLvoid* pointer) {bind();normalPointer(type, stride, pointer);}
  void bindColorPointer(GLsizei stride=0, const GLvoid* pointer=0) {bind();colorPointer(size, type, stride, pointer);}

  void draw(GLenum mode, GLint first=0, GLsizei count=-1)
  {
    if(count == -1)
      count = numElements;
    glDrawArrays(mode, first, count);GLDEBUG_THROW;
  }
};
}
}

#endif
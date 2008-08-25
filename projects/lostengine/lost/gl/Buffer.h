#ifndef LOST_GL_BUFFER_H
#define LOST_GL_BUFFER_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

namespace lost
{
namespace gl
{
struct Buffer
{
  GLuint buffer;
  GLenum target;
  GLint size;
  GLenum type;  
  unsigned long numElements; // number of verts/colors/texcoords in array

  Buffer()
  {
    glGenBuffers(1, &buffer);GLDEBUG_THROW;
    numElements = 0;        
  }
  
  virtual ~Buffer()
  {
    glDeleteBuffers(1, &buffer);GLDEBUG;// must not throw    
  }

  virtual void bind() { Buffer::bind(target); }
  virtual void unbind() { glBindBuffer(target, 0); }
  virtual void bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage=GL_STATIC_DRAW) {Buffer::bufferData(target, size, data, usage);}
  virtual void bindBufferData(GLsizeiptr size, const GLvoid* data, GLenum usage=GL_STATIC_DRAW) {bind();Buffer::bufferData(target, size, data, usage);}

  virtual void bindVertexPointer(GLsizei stride=0, const GLvoid* pointer=0){bind();vertexPointer(size, type, stride, pointer);}
  virtual void bindTexCoordPointer(GLsizei stride=0, const GLvoid* pointer=0) {bind();texCoordPointer(size, type, stride, pointer);}
  virtual void bindNormalPointer(GLsizei stride=0, const GLvoid* pointer=0) {bind();normalPointer(type, stride, pointer);}
  virtual void bindColorPointer(GLsizei stride=0, const GLvoid* pointer=0) {bind();colorPointer(size, type, stride, pointer);}

  virtual void drawArrays(GLenum mode, GLint first=0, GLsizei count=-1)
  {
    if(count == -1)
      count = numElements;
    glDrawArrays(mode, first, count);GLDEBUG_THROW;
  }

  // indices: used as byte offset into buffer when element array is bound
  virtual void drawElements(GLenum mode, GLsizei count=-1, const GLvoid* indices = 0)
  {
    if(count == -1)
      count = numElements;
    drawElements(mode, count, this->type, indices);
  }
  
  virtual void drawElements(GLenum  	     mode,
                            GLsizei  	     count,
                            GLenum  	     inType,
                            const GLvoid * indices)
  {
    glDrawElements(mode, count, inType, indices);GLDEBUG_THROW;
  }
  
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
  // indices: used as byte offset into buffer when element array is bound
  virtual void drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count=-1, const GLvoid* indices = 0)
  {
    if(count == -1)
      count = numElements;
    drawRangeElements(mode, start, end, count, this->type, indices);
  }
  
  virtual void drawRangeElements(GLenum      	  mode,
                                 GLuint         start,
                                 GLuint         end,
                                 GLsizei        count,
                                 GLenum  	      inType,
                                 const GLvoid * indices)
  {
    glDrawRangeElements(mode, start, end, count, inType, indices);GLDEBUG_THROW;
  }
#endif
  
  /**
   * GL_ARRAY_BUFFER
   * GL_ELEMENT_ARRAY_BUFFER
   * GL_PIXEL_PACK_BUFFER
   * GL_PIXEL_UNPACK_BUFFER
   */
  virtual void bind(GLenum target)
  {
    glBindBuffer(target, buffer);GLDEBUG_THROW;
  }
  
  /**
   * target: same targets as bind
   * size size in bytes of objects new store
   * data: pointer to data to be copied
   * usage: one of the following
   *  GL_STREAM_DRAW
   *  GL_STREAM_READ
   *  GL_STREAM_COPY
   *  GL_STATIC_DRAW
   *  GL_STATIC_READ
   *  GL_STATIC_COPY
   *  GL_DYNAMIC_DRAW
   *  GL_DYNAMIC_READ
   *  GL_DYNAMIC_COPY
   usage is a hint to the GL implementation as to how a buffer object's data store will be accessed. This enables the GL implementation to make more intelligent decisions that may significantly impact buffer object performance. It does not, however, constrain the actual usage of the data store. usage can be broken down into two parts: first, the frequency of access (modification and usage), and second, the nature of that access. The frequency of access may be one of these:
   STREAM
   The data store contents will be modified once and used at most a few times.
   STATIC
   The data store contents will be modified once and used many times.
   DYNAMIC
   The data store contents will be modified repeatedly and used many times.
   
   The nature of access may be one of these:
   DRAW
   The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
   READ
   The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
   COPY
   The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.   
   */
  virtual void bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
  {
    glBufferData(target, size, data, usage);GLDEBUG_THROW;
  }
  
  /**
   * size: number of coords per vertex, must be 2, 3 or 4
   * type: data type of each coord in array: GL_SHORT, GL_INT, GL_FLOAT, GL_DOUBLE
   * stride: byte offset between consecutive vertices, default 0
   * pointer to first coord in array, default 0
   */
  virtual void vertexPointer(GLint size, GLenum type, GLsizei stride=0, const GLvoid* pointer=0)
  {
    glVertexPointer(size, type, stride, pointer);GLDEBUG_THROW;
  }
  
  // size: 1,2,3 or 4
  virtual void texCoordPointer(GLint size, GLenum type, GLsizei stride=0, const GLvoid* pointer=0)
  {
    glTexCoordPointer(size, type, stride, pointer);GLDEBUG_THROW;
  }
  
  // always 3 coords?
  virtual void normalPointer(GLenum type, GLsizei stride=0, const GLvoid* pointer=0)
  {
    glNormalPointer(type, stride, pointer);GLDEBUG_THROW;
  }
  
  // size: 3 or 4
  // type: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE
  virtual void colorPointer(GLint size, GLenum type, GLsizei stride=0, const GLvoid* pointer=0)
  {
    glColorPointer(size, type, stride, pointer);GLDEBUG_THROW;
  }
  
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
  /**
   * index_ index of generic vertex attribute
   * size: number of components per attribute, 1,2,3 or 4
   * type: type of each component: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE
   * normalized: normalize fixed-point data (GL_TRUE) or directly convert to fixed point
   */
  virtual void vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride=0, const GLvoid* pointer=0)
  {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);GLDEBUG_THROW; 
  }
#endif
  
};
}
}

#endif
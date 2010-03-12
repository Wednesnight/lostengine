#include "lost/gl/Buffer.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"


namespace lost
{
namespace gl
{


  Buffer::Buffer()
  {
    glGenBuffers(1, &buffer);GLDEBUG_THROW;
    numElements = 0;
  }

   Buffer::~Buffer()
  {
    glDeleteBuffers(1, &buffer);GLDEBUG;// must not throw
  }

   void Buffer::bind() { Buffer::bind(target); }
   void Buffer::unbind() { glBindBuffer(target, 0); }
   void Buffer::bufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {Buffer::bufferData(target, size, data, usage);}
   void Buffer::bindBufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {bind();Buffer::bufferData(target, size, data, usage);}
   void Buffer::bufferSubData(GLintptr offset,GLsizeiptr sizeInBytes,	const GLvoid* data) {Buffer::bufferSubData(target, offset, size, data);};
   void Buffer::bindBufferSubData(GLintptr offset,GLsizeiptr size,	const GLvoid* data) {bind(); Buffer::bufferSubData(target, offset, size, data);};

/*
//  virtual void bindIndexPointer(GLsizei stride=0, const GLvoid* pointer=0){bind();indexPointer(stride, pointer);}
   void Buffer::bindVertexPointer(GLsizei stride, const GLvoid* pointer){bind();vertexPointer(stride, pointer);}
   void Buffer::bindTexCoordPointer(GLsizei stride, const GLvoid* pointer) {bind();texCoordPointer(stride, pointer);}
   void Buffer::bindNormalPointer(GLsizei stride, const GLvoid* pointer) {bind();normalPointer(stride, pointer);}
   void Buffer::bindColorPointer(GLsizei stride, const GLvoid* pointer) {bind();colorPointer(stride, pointer);}
*/
   void Buffer::drawArrays(GLenum mode, GLint first, GLsizei count)
  {
    if(count == -1)
      count = numElements;
    glDrawArrays(mode, first, count);GLDEBUG_THROW;
  }

  // indices: used as byte offset into buffer when element array is bound
   void Buffer::drawElements(GLenum mode, GLsizei count, const GLvoid* indices)
  {
    if(count == -1)
      count = numElements;
    drawElements(mode, count, this->type, indices);
  }

   void Buffer::drawElements(GLenum  	     mode,
                            GLsizei  	     count,
                            GLenum  	     inType,
                            const GLvoid * indices)
  {
    glDrawElements(mode, count, inType, indices);GLDEBUG_THROW;
  }

/*  // indices: used as byte offset into buffer when element array is bound
   void Buffer::drawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, const GLvoid* indices)
  {
    if(count == -1)
      count = numElements;
    drawRangeElements(mode, start, end, count, this->type, indices);
  }

   void Buffer::drawRangeElements(GLenum      	  mode,
                                 GLuint         start,
                                 GLuint         end,
                                 GLsizei        count,
                                 GLenum  	      inType,
                                 const GLvoid * indices)
  {
    glDrawRangeElements(mode, start, end, count, inType, indices);GLDEBUG_THROW;
  }
*/

  /**
   * GL_ARRAY_BUFFER
   * GL_ELEMENT_ARRAY_BUFFER
   * GL_PIXEL_PACK_BUFFER
   * GL_PIXEL_UNPACK_BUFFER
   */
   void Buffer::bind(GLenum inTarget)
  {
    glBindBuffer(inTarget, buffer);GLDEBUG_THROW;
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
   void Buffer::bufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
  {
    glBufferData(target, size, data, usage);GLDEBUG_THROW;
  }

   void Buffer::bufferSubData(GLenum  	target,
                             GLintptr  	offset,
                             GLsizeiptr  	size,
                             const GLvoid *  	data)
  {
    glBufferSubData(target, offset, size, data);GLDEBUG_THROW;
  }


/*  void indexPointer(GLsizei  	stride=0,
                    const GLvoid *  	pointer=0)
  {
    glIndexPointer(type, stride, pointer);GLDEBUG_THROW;
  }*/

  /**
   * size: number of coords per vertex, must be 2, 3 or 4
   * type: data type of each coord in array: GL_SHORT, GL_INT, GL_FLOAT, GL_DOUBLE
   * stride: byte offset between consecutive vertices, default 0
   * pointer to first coord in array, default 0
   */
/*   void Buffer::vertexPointer(GLsizei stride, const GLvoid* pointer)
  {
    glVertexPointer(size, type, stride, pointer);GLDEBUG_THROW;
  }*/

  // size: 1,2,3 or 4
/*   void Buffer::texCoordPointer(GLsizei stride, const GLvoid* pointer)
  {
    glTexCoordPointer(size, type, stride, pointer);GLDEBUG_THROW;
  }*/

  // always 3 coords?
/*   void Buffer::normalPointer(GLsizei stride, const GLvoid* pointer)
  {
    glNormalPointer(type, stride, pointer);GLDEBUG_THROW;
  }*/

  // size: 3 or 4
  // type: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE
/*   void Buffer::colorPointer(GLsizei stride, const GLvoid* pointer)
  {
    glColorPointer(size, type, stride, pointer);GLDEBUG_THROW;
  }*/

  /**
   * index_ index of generic vertex attribute
   * size: number of components per attribute, 1,2,3 or 4
   * type: type of each component: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE
   * normalized: normalize fixed-point data (GL_TRUE) or directly convert to fixed point
   */
   void Buffer::vertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
  {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);GLDEBUG_THROW;
  }

}
}
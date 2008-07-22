#ifndef LOST_GL_TEXTURE_H
#define LOST_GL_TEXTURE_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include <stdexcept>
#include <boost/noncopyable.hpp>
#include "lost/bitmap/Bitmap.h"

namespace lost
{
  namespace gl
  {
    /** a 2D Texture helper class.
     *
     */
    struct Texture : private boost::noncopyable
    {
      Texture()
      {
        glGenTextures(1, &texture);GLDEBUG_THROW;
      }
      
      ~Texture()
      {
        glDeleteTextures(1, &texture);GLDEBUG;
      }
      
      void bind() const
      {
        glBindTexture(GL_TEXTURE_2D, texture);GLDEBUG_THROW;       
      }

      void reset(GLint level,
                 GLenum internalformat,
                 bool border,
                 const lost::bitmap::Bitmap& inBitmap)
      {
        reset(level,
              internalformat, 
              inBitmap.width,
              inBitmap.height,
              border ? 1 : 0,
              inBitmap.format,
              inBitmap.type,
              inBitmap.data);
        width = inBitmap.width;
        height = inBitmap.height;
      }

      void reset(GLint level, // mipmap level
                 GLenum internalformat, // number of color components
                 GLsizei width,
                 GLsizei height,
                 GLint border, // must be 0 or 1 
                 GLenum format, // composition of each element in pixels 
                 GLenum type, // numerical type of provided pixel data
                 const GLvoid* data) // pointer to the data or 0 if you only want to reserve data for later usage
      {
          glTexImage2D(GL_TEXTURE_2D,
                       level,
                       internalformat,
                       width,
                       height,
                       border,
                       format,
                       type,
                       data);GLDEBUG_THROW;      
      }

      void wrap(GLint p) { wrapS(p); wrapT(p);}
      void wrapS(GLint p) { param(GL_TEXTURE_WRAP_S, p); }
      void wrapT(GLint p) { param(GL_TEXTURE_WRAP_T, p); }

      void filter(GLint p)  {minFilter(p); magFilter(p); }
      void minFilter(GLint p) {param(GL_TEXTURE_MIN_FILTER, p);}
      void magFilter(GLint p) {param(GL_TEXTURE_MAG_FILTER, p);}
  
      void param(GLenum pname, GLint p) {glTexParameteri(GL_TEXTURE_2D, pname, p);}      
                      
      GLuint        texture;
      // original width and height of the bitmap this texture was created from
      int width;
      int height;      
    };
  }
}

#endif



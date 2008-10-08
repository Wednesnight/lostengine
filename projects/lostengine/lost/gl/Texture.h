#ifndef LOST_GL_TEXTURE_H
#define LOST_GL_TEXTURE_H

#include "lost/gl/gl.h"
#include <boost/noncopyable.hpp>

namespace boost { template<typename T> class shared_ptr; }

namespace lost
{
  namespace bitmap { struct Bitmap; };

  namespace gl
  {
    /** a 2D Texture helper class.
     *
     */
    struct Texture : private boost::noncopyable
    {
      struct Params
      {
        GLint   level;          // mipmap level
        GLenum  internalFormat; // number of color components 
        GLsizei width;          // texture width
        GLsizei height;         // texture height
        GLint   border;         // 1 = with border, 0 = without
        GLenum  format;         // format of the incoming bitmap data: number and kind of components, sequence
        GLenum  type;           // type of the color components
      };
    
      Texture();
      ~Texture();    
      void destroy();  
      void bind() const;      
      void reset(GLint level,
                 GLenum internalFormat,
                 bool border,
                 boost::shared_ptr<lost::bitmap::Bitmap> inBitmap);
      void reset(GLint level, // mipmap level
                 GLenum internalformat, // number of color components
                 GLsizei width,
                 GLsizei height,
                 GLint border, // must be 0 or 1 
                 GLenum format, // composition of each element in pixels 
                 GLenum type, // numerical type of provided pixel data
                 const GLvoid* data); // pointer to the data or 0 if you only want to reserve data for later usage

      void wrap(GLint p);
      void wrapS(GLint p);
      void wrapT(GLint p);

      void filter(GLint p);
      void minFilter(GLint p);
      void magFilter(GLint p);
  
      void param(GLenum pname, GLint p);
                      
      GLuint        texture;
      int width;
      int height;      
    };
  }
}

#endif



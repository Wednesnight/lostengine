#ifndef LOST_GL_TEXTURE_H
#define LOST_GL_TEXTURE_H

#include "lost/gl/gl.h"
#include <boost/noncopyable.hpp>

namespace boost { template<typename T> class shared_ptr; }

namespace lost
{
  namespace bitmap { struct Bitmap; };
  namespace resource { struct File; };

  namespace gl
  {
    /** a 2D Texture helper class.
     *
     */
    struct Texture : private boost::noncopyable
    {
      enum SizeHint
      {
        SIZE_ORIGINAL = 0,
        SIZE_POWER_OF_TWO,
        SIZE_DONT_CARE,
        SIZE_NUM
      };
      
      struct Params
      {
        Params();
        
        GLint     level;          // mipmap level
        GLenum    internalFormat; // number of color components 
        GLint     border;         // 1 = with border, 0 = without
        GLenum    format;         // format of the incoming bitmap data: number and kind of components, sequence
        GLenum    type;           // type of the color components
        GLint     wrapS;          // default s wrapping mode that will be set after construction
        GLint     wrapT;          // default t wrapping mode that will be set after construction
        GLint     minFilter;      // default minification filter that will be set after construction
        GLint     magFilter;      // default magnification filter that will be set after construction
        SizeHint  sizeHint;       // size for constructing power of two textures depending on the capabilities of the platform
                                  // with this flag, you can force (non) power of two textures or just leave it up
                                  // to the Texture class to decide what's best
      };
    
      Texture();
      Texture(boost::shared_ptr<lost::resource::File> inFile,  const Params& inParams = Params());
      Texture(boost::shared_ptr<lost::bitmap::Bitmap> inBitmap, const Params& inParams = Params());      
      ~Texture();    
      void destroy();  
      void bind() const;      
      
      void init(boost::shared_ptr<lost::resource::File> inFile,  const Params& inParams = Params());
      void init(boost::shared_ptr<lost::bitmap::Bitmap> inBitmap, const Params& inParams = Params());
      
      void init(GLint level, // mipmap level
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
      // width and height of the texture object
      uint32_t width;
      uint32_t height;      
      uint32_t dataWidth; 
      uint32_t dataHeight;
    private:
      void create();
    };
  }
}

#endif



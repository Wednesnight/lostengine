#include "lost/gl/Texture.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/Utils.h"
#include <stdexcept>


namespace lost
{
namespace gl
{

Texture::Texture()
{
  glGenTextures(1, &texture);GLDEBUG_THROW;
}

Texture::~Texture()
{
  destroy();
}

void Texture::destroy()
{
  glDeleteTextures(1, &texture);GLDEBUG;
}

void Texture::bind() const
{
  glBindTexture(GL_TEXTURE_2D, texture);GLDEBUG_THROW;       
}

void Texture::reset(GLint level,
           GLenum internalFormat,
           bool border,
           boost::shared_ptr<lost::bitmap::Bitmap> inBitmap)
{
  reset(level,
        internalFormat, 
        inBitmap->width,
        inBitmap->height,
        border ? 1 : 0,
        inBitmap->format,
        inBitmap->type,
        inBitmap->data);
  width = inBitmap->width;
  height = inBitmap->height;
}

void Texture::reset(GLint level, // mipmap level
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

void Texture::wrap(GLint p)
{
  wrapS(p);
  wrapT(p);
}

void Texture::wrapS(GLint p)
{
  param(GL_TEXTURE_WRAP_S, p);
}

void Texture::wrapT(GLint p)
{
  param(GL_TEXTURE_WRAP_T, p);
}

void Texture::filter(GLint p)
{
  minFilter(p);
  magFilter(p);
}

void Texture::minFilter(GLint p)
{
  param(GL_TEXTURE_MIN_FILTER, p);
}

void Texture::magFilter(GLint p)
{
  param(GL_TEXTURE_MAG_FILTER, p);
}

void Texture::param(GLenum pname, GLint p)
{
  glTexParameteri(GL_TEXTURE_2D, pname, p);GLDEBUG_THROW;
}      

}
}
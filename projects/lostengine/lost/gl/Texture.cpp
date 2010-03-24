#include "lost/gl/Texture.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/Utils.h"
#include "lost/gl/gl.h"
#include <stdexcept>
#include "lost/math/lmath.h"
#include "lost/gl/Context.h"

using namespace std;
using namespace boost;
using namespace lost::bitmap;

namespace lost
{
namespace gl
{

Texture::Params::Params()
{
  level = 0;
  internalFormat = GL_RGBA;
  border = 0;
  format = GL_RGBA;
  type = GL_UNSIGNED_BYTE;
  wrapS = GL_CLAMP_TO_EDGE;
  wrapT = GL_CLAMP_TO_EDGE;
  minFilter = GL_NEAREST;
  magFilter = GL_NEAREST;
  sizeHint = SIZE_DONT_CARE;
}

GLenum bitmapComponents2GlFormat(bitmap::Components components)
{
  GLenum result = 0;
  switch(components)
  {
    case bitmap::COMPONENTS_RGB:result=GL_RGB;break;
    case bitmap::COMPONENTS_RGBA:result=GL_RGBA;break;
    case bitmap::COMPONENTS_ALPHA:result=GL_ALPHA;break;
    default:
      EOUT("throwing from here");
      std::ostringstream os;
      os << "can't convert bitmap components " << components;
      throw runtime_error(os.str());
  }
  return result;
}

void Texture::create()
{
  glGenTextures(1, &texture);GLDEBUG_THROW;
}


Texture::Texture()
{
  create();
}

Texture::Texture(const lost::math::Vec2& inSize, const Params& inParams)
{
  create();
  init(inSize, inParams);
}

Texture::Texture(common::DataPtr inData,  const Params& inParams)
{
  create();
  init(inData, inParams);
}

Texture::Texture(bitmap::BitmapPtr inBitmap, const Params& inParams)
{
  create();
  init(inBitmap, inParams);
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
//  glBindTexture(GL_TEXTURE_2D, texture); GLDEBUG_THROW;
  Context::getCurrent()->bindTexture(texture);
}

void Texture::unbind() const
{
  // reset to default texture
//  glBindTexture(GL_TEXTURE_2D, 0); GLDEBUG_THROW;
  Context::getCurrent()->bindTexture(0);
}

void Texture::init(common::DataPtr inData,  const Params& inParams)
{
  BitmapPtr bmp(new Bitmap(inData));
  init(bmp, inParams);
}

void Texture::init(const lost::math::Vec2& inSize, const Texture::Params& inParams)
{
  bind();
  
  uint32_t texwidth, texheight;
  Texture::SizeHint sizeHint = inParams.sizeHint;
  // if sizehint is dontcare we try to choose non-power-of-two, unless the platform doesn't allow it
  if(sizeHint == Texture::SIZE_DONT_CARE)
  {
#if TARGET_OPENGL
    if(GLEE_ARB_texture_non_power_of_two)
      sizeHint = Texture::SIZE_ORIGINAL;
    else
#endif
      sizeHint = Texture::SIZE_POWER_OF_TWO;
  }

  // then we calculate the size of the texture object
  if(sizeHint == Texture::SIZE_ORIGINAL)
  {
    texwidth = (uint32_t)inSize.width;
    texheight = (uint32_t)inSize.height;
  }
  else if(sizeHint == Texture::SIZE_POWER_OF_TWO)
  {
    texwidth = lost::math::nextPowerOf2((unsigned long)inSize.width);
    texheight = lost::math::nextPowerOf2((unsigned long)inSize.height);
  }

  // create an empty texture object, i.e. without data, to setup the desired size
  init(inParams.level,
        inParams.internalFormat,
        texwidth,
        texheight,
        inParams.border ? 1 : 0,
        inParams.format,
        inParams.type,
        0);

  // memorize texture and raw data sizes for texture coordinate calculations
  width = texwidth;
  height = texheight;
  dataWidth = (uint32_t)inSize.width;
  dataHeight = (uint32_t)inSize.height;
}

void Texture::init(bitmap::BitmapPtr inBitmap, const Texture::Params& inParams)
{
  Texture::Params bitmapParams(inParams);
  bitmapParams.internalFormat = bitmapParams.format = bitmapComponents2GlFormat(inBitmap->format); // FIXME: is this correct?

  // creates texture with correct size according to size hint
  init(lost::math::Vec2((float)inBitmap->width, (float)inBitmap->height), bitmapParams);

  // then use texsubimage to upload the actual data. Strictly speaking, we only need this in the
  // case of power-of-two textures, but always creating textures like this saves us one branch
  // now we copy the actual data to the previously allocated texture
  glTexSubImage2D(GL_TEXTURE_2D,
                  inParams.level,
                  0,
                  0,
                  inBitmap->width,
                  inBitmap->height,
                  bitmapParams.format,
                  inParams.type,
                  inBitmap->data);GLDEBUG_THROW;

  // set wrapping and filters
  wrapS(bitmapParams.wrapS);
  wrapT(bitmapParams.wrapT);
  minFilter(bitmapParams.minFilter);
  magFilter(bitmapParams.magFilter);

  dataWidth = inBitmap->width;
  dataHeight = inBitmap->height;

}

void Texture::init(const std::vector<bitmap::BitmapPtr>& inBitmaps, const Params& inParams)
{
  bind();  
  Texture::Params bitmapParams(inParams);
  bitmapParams.level = 0;
  bitmapParams.internalFormat = bitmapParams.format = bitmapComponents2GlFormat(inBitmaps[0]->format); // FIXME: is this correct?

  for(uint32_t i=0; i<inBitmaps.size(); ++i)
  {
    glTexImage2D(GL_TEXTURE_2D,
             bitmapParams.level,
             bitmapParams.internalFormat,
             inBitmaps[i]->width,
             inBitmaps[i]->height,
             bitmapParams.border,
             bitmapParams.format,
             bitmapParams.type,
             inBitmaps[i]->data);GLDEBUG_THROW;
    ++bitmapParams.level;
  }
  
  wrapS(bitmapParams.wrapS);
  wrapT(bitmapParams.wrapT);
  minFilter(bitmapParams.minFilter);
  magFilter(bitmapParams.magFilter);

  // size hint is ignored for now, size of first texture is used, assuming power-of-two and complete usage of area
  width = dataWidth = inBitmaps[0]->width;
  height = dataHeight = inBitmaps[0]->height;  
}

void Texture::subImage(const lost::math::Vec2& targetPos, bitmap::BitmapPtr inBitmap)
{
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  (GLint)targetPos.x,
                  (GLint)targetPos.y,
                  inBitmap->width,
                  inBitmap->height,
                  bitmapComponents2GlFormat(inBitmap->format),
                  GL_UNSIGNED_BYTE,
                  inBitmap->data);GLDEBUG_THROW;  
}

void Texture::init(GLint level, // mipmap level
           GLenum internalformat, // number of color components
           GLsizei width,
           GLsizei height,
           GLint border, // must be 0 or 1
           GLenum format, // composition of each element in pixels
           GLenum type, // numerical type of provided pixel data
           const GLvoid* data) // pointer to the data or 0 if you only want to reserve data for later usage
{
    bind();
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

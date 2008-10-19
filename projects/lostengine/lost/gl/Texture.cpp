#include "lost/gl/Texture.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/Utils.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include "lost/math/lmath.h"
#include "lost/resource/File.h"

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
  
GLenum bitmapComponents2GlFormat(bitmap::Bitmap::Components components)
{
  GLenum result = 0;
  switch(components)
  {
    case bitmap::Bitmap::COMPONENTS_RGB:result=GL_RGB;break;
    case bitmap::Bitmap::COMPONENTS_RGBA:result=GL_RGBA;break;
    case bitmap::Bitmap::COMPONENTS_ALPHA:result=GL_ALPHA;break;
    default:DOUT("throwing from her");throw runtime_error(string("can't convert bitmap components ")+lexical_cast<string>(components));
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

Texture::Texture(boost::shared_ptr<lost::resource::File> inFile,  const Params& inParams)
{
  create();
  init(inFile, inParams);
}

Texture::Texture(boost::shared_ptr<lost::bitmap::Bitmap> inBitmap, const Params& inParams)
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
  glBindTexture(GL_TEXTURE_2D, texture);GLDEBUG_THROW;       
}

  void Texture::init(boost::shared_ptr<lost::resource::File> inFile,  const Params& inParams)
{
  shared_ptr<Bitmap> bmp(new Bitmap(inFile));
  init(bmp, inParams);
}

void Texture::init(boost::shared_ptr<lost::bitmap::Bitmap> inBitmap, const Texture::Params& inParams)
{
  uint32_t texwidth, texheight;
  Texture::SizeHint sizeHint = inParams.sizeHint;
  // if sizehint is dontcare we try to choose non-power-of-two, unless the platform doesn't allow it
  if(sizeHint == Texture::SIZE_DONT_CARE)
  {
    if(GLEE_ARB_texture_non_power_of_two)
      sizeHint = Texture::SIZE_ORIGINAL;
    else
      sizeHint = Texture::SIZE_POWER_OF_TWO;
  }

  // then we calculate the size of the texture object
  if(sizeHint == Texture::SIZE_ORIGINAL)
  {
    texwidth = inBitmap->width;
    texheight = inBitmap->height;
  }
  else if(sizeHint == Texture::SIZE_POWER_OF_TWO)
  {
    texwidth = lost::math::nextPowerOf2(inBitmap->width);
    texheight = lost::math::nextPowerOf2(inBitmap->height);
  }
 
  // create an empty texture object, i.e. without data, to setup the desired size
  init(inParams.level,
        inParams.internalFormat, 
        texwidth,
        texheight,
        inParams.border ? 1 : 0,
        bitmapComponents2GlFormat(inBitmap->format),
        GL_UNSIGNED_BYTE,
        0);
  
  // then use texsubimage to upload the actual data. Strictly speaking, we only need this in the
  // case of power-of-two textures, but always creating textures like this saves us one branch
  // now we copy the actual data to the previsouly allocated texture
  glTexSubImage2D(GL_TEXTURE_2D,
                  0,
                  0,
                  0,
                  inBitmap->width,
                  inBitmap->height,
                  bitmapComponents2GlFormat(inBitmap->format),
                  GL_UNSIGNED_BYTE,
                  inBitmap->data);GLDEBUG_THROW;
  
  // set wrapping and filters
  wrapS(inParams.wrapS);
  wrapT(inParams.wrapT);
  minFilter(inParams.minFilter);
  magFilter(inParams.magFilter);
  
  // memorize texture and raw data sizes for texture coordinate calculations
  width = texwidth;
  height = texheight;
  dataWidth = inBitmap->width;
  dataHeight = inBitmap->height;
  
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
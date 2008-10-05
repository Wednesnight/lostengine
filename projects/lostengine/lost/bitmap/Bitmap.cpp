#include "lost/bitmap/Bitmap.h"
#include "stb_image.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include "lost/resource/File.h"

namespace lost
{
namespace bitmap
{

void Bitmap::init()
{
  data = NULL;
  loaded = false;
  width = 0;
  height = 0;
  format = 0;
  type = 0;
}

Bitmap::Bitmap()
{
  init();
}

Bitmap::Bitmap(unsigned long inWidth,
          unsigned long inHeight)
{
  init();
  init(inWidth, inHeight);
}
          
Bitmap::Bitmap(boost::shared_ptr<lost::resource::File> inFile)
{
  init();
  init(inFile);
}


Bitmap::~Bitmap()
{
  destroy();
}

void Bitmap::destroy()
{
  if(data && loaded)
  {
    DOUT("stbi_image_free");
    stbi_image_free(data);
  }
  else if(data && !loaded)
  {
    DOUT("delete");
    delete [] data;
  }
}

void Bitmap::init(unsigned long inWidth,
                  unsigned long inHeight)
{
  destroy();
  unsigned long bytesPerPixel = 4;
  unsigned long sizeInBytes = inWidth * inHeight * bytesPerPixel;
  data = new unsigned char[sizeInBytes];
  loaded = false;
  format = GL_RGBA;
  type = GL_UNSIGNED_BYTE;
}

void Bitmap::init(boost::shared_ptr<lost::resource::File> inFile)
{
  destroy();
  DOUT("init image from memory: " << inFile->location);
  int bytesPerPixel, w, h;
  data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(inFile->data.get()), inFile->size, &w, &h, &bytesPerPixel, 0);
  width = w;
  height = h;
  if(data == NULL)
  {
      throw std::runtime_error("couldn't init image from memory: " + inFile->location);      
  }
      
  switch(bytesPerPixel)
  {
    case 3:format = GL_RGB;break;
    case 4:format = GL_RGBA;break;
    default:throw std::runtime_error("couldn't init image: "+inFile->location+" don't know what to do with bytesPerPixel: "+boost::lexical_cast<std::string>(bytesPerPixel));
  }
  type = GL_UNSIGNED_BYTE;  
  loaded = true;
}

} 
} 

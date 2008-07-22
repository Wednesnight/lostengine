#include "lost/bitmap/Bitmap.h"
#include "stb_image.h"
#include "lost/common/Logger.h"
#include <stdexcept>

namespace lost
{
namespace bitmap
{

Bitmap::Bitmap()
{
  data = NULL;
}

Bitmap::~Bitmap()
{
  if(data)
  {
    stbi_image_free(data);
  }
}

void Bitmap::init(boost::shared_ptr<lost::resource::File> inFile)
{
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
}

} 
} 

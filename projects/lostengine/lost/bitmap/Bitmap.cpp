#include "lost/bitmap/Bitmap.h"
#include "stb_image.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include "lost/resource/File.h"
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

namespace lost
{
namespace bitmap
{

void Bitmap::reset()
{
  data = NULL;
  loaded = false;
  width = 0;
  height = 0;
  format = COMPONENTS_UNDEFINED;
}

Bitmap::Bitmap()
{
  reset();
}

Bitmap::Bitmap(uint32_t inWidth,
               uint32_t inHeight,
               Components destComponents,
               Components srcComponents,
               uint8_t* data)
{
  reset();
  init(inWidth, inHeight, destComponents, srcComponents, data);
}
          
Bitmap::Bitmap(boost::shared_ptr<lost::resource::File> inFile)
{
  reset();
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
  reset();
}

void Bitmap::init(uint32_t inWidth,
                  uint32_t inHeight,
                  Components destComponents,
                  Components srcComponents,
                  uint8_t* srcData)
{
  destroy();
  
  // create target memory
  uint32_t destBytesPerPixel = bytesPerPixelFromComponents(destComponents);
  uint32_t destSizeInBytes = destBytesPerPixel * inWidth * inHeight;
  data = new uint8_t[destSizeInBytes];
  loaded = false; // prevent stb_image from freeing
  
  // setup stc values for loop
  uint32_t srcBytesPerPixel = bytesPerPixelFromComponents(srcComponents);
  uint8_t* destWriter = data;
  uint8_t* srcReader = srcData;
  uint32_t numPixels = inWidth*inHeight;

  // copy pixels
  for(uint32_t currentPixel=0; currentPixel<numPixels; ++currentPixel)
  {
    copyPixel(destWriter, destComponents, srcReader, srcComponents);
    destWriter+=destBytesPerPixel;
    srcReader+=srcBytesPerPixel;
  }
  
  width = inWidth;
  height = inHeight;
  format = destComponents;
}

// FIXME: not endian safe, we nee dto fix this for big endian
// FIXME: not sure if this is even correct byte order for little endian
  void Bitmap::copyPixel(uint8_t* dest,
                Components destComponents, 
                uint8_t* src,
                Components srcComponents)
{
  switch(destComponents)
  {
    case COMPONENTS_RGBA:
      switch(srcComponents)
      {
        case COMPONENTS_RGBA:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];dest[3]=src[3];break;
        case COMPONENTS_RGB:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];dest[3]=255;break;
        case COMPONENTS_ALPHA:dest[0]=src[0];dest[1]=src[0];dest[2]=src[0];dest[3]=src[0];break;
        default:throw runtime_error("can't copy pixel from source with components: "+lexical_cast<string>(srcComponents));
      }      
      break;
    case COMPONENTS_RGB:
      switch(srcComponents)
      {
        case COMPONENTS_RGBA:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];break;
        case COMPONENTS_RGB:dest[0]=src[0];dest[1]=src[1];dest[2]=src[2];break;
        case COMPONENTS_ALPHA:dest[0]=src[0];dest[1]=src[0];dest[2]=src[0];break;
        default:throw runtime_error("can't copy pixel from source with components: "+lexical_cast<string>(srcComponents));
      }
      break;
    case COMPONENTS_ALPHA:
      switch(srcComponents)
      {
        case COMPONENTS_RGBA:dest[0]=src[3];break;
        case COMPONENTS_RGB:dest[0]=255;break;
        case COMPONENTS_ALPHA:dest[0]=src[0];break;
        default:throw runtime_error("can't copy pixel from source with components: "+lexical_cast<string>(srcComponents));
      }
      break;
    default:throw runtime_error("can't copy pixel to destination with components: "+lexical_cast<string>(destComponents));
  }
}
  
uint32_t Bitmap::bytesPerPixelFromComponents(Components components)
{
  uint32_t result = 0;
  switch(components)
  {
    case COMPONENTS_ALPHA:result = 1;break;
    case COMPONENTS_RGB:result = 3;break;
    case COMPONENTS_RGBA:result = 4;break;
    default:throw runtime_error("can't determine bytes per pixel from components: "+lexical_cast<string>(components));
  }
  return result;
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
    case 3:format = COMPONENTS_RGB;break;
    case 4:format = COMPONENTS_RGBA;break;
    default:throw std::runtime_error("couldn't init image: "+inFile->location+" don't know what to do with bytesPerPixel: "+boost::lexical_cast<std::string>(bytesPerPixel));
  }
  loaded = true;
}

} 
} 

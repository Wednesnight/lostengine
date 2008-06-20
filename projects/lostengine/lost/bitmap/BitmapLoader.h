#ifndef LOST_BITMAP_IMAGELOADER_H
#define LOST_BITMAP_IMAGELOADER_H

#include <boost/shared_ptr.hpp>
#include "lost/bitmap/Bitmap.h"
#include "lost/common/Logger.h"
#include "lost/resource/File.h"
#include "lost/platform/Platform.h"
#include <sstream>
#include <stdexcept>

namespace lost
{

namespace bitmap
{

struct BitmapLoader
{
  BitmapLoader()
  {
    DOUT("creating");
  }

  ~BitmapLoader()
  {
    DOUT("destroying");
  }

  boost::shared_ptr<lost::bitmap::Bitmap> loadFromResourcePath(const std::string& inRelativePath)
  {
    return loadFromPath(lost::platform::buildResourcePath(inRelativePath));
  }
  

  boost::shared_ptr<lost::bitmap::Bitmap> loadFromPath(const std::string& inPathToFile)
  {
    DOUT("loading image: " << inPathToFile);
    int width, height, bytesPerPixel;
    unsigned char* data = stbi_load(inPathToFile.c_str(), &width, &height, &bytesPerPixel, 0);
    if(data == NULL)
    {
        throw std::runtime_error("couldn't load image: "+inPathToFile);      
    }
  
    return createAndInitWith(inPathToFile, data, width, height, bytesPerPixel);
  }

  boost::shared_ptr<lost::bitmap::Bitmap> loadFromMemory(boost::shared_ptr<lost::resource::File> inFile)
  {
    DOUT("loading image from memory: " << inFile->location);
    int width, height, bytesPerPixel;
    unsigned char* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(inFile->data.get()), inFile->size, &width, &height, &bytesPerPixel, 0);
    if(data == NULL)
    {
        throw std::runtime_error("couldn't load image from memory: " + inFile->location);      
    }
  
    return createAndInitWith(inFile->location, data, width, height, bytesPerPixel);
  }

  boost::shared_ptr<lost::bitmap::Bitmap> createAndInitWith(const std::string location, unsigned char* data, int width, int height, int bytesPerPixel)
  {
    boost::shared_ptr<lost::bitmap::Bitmap> result(new lost::bitmap::Bitmap());
    result->data = data;
    result->width = width;
    result->height = height;
        
    switch(bytesPerPixel)
    {
      case 3:result->format = GL_RGB;break;
      case 4:result->format = GL_RGBA;break;
      default:throw std::runtime_error("couldn't load image: "+location+" don't know what to do with bytesPerPixel: "+boost::lexical_cast<std::string>(bytesPerPixel));
    }
    result->type = GL_UNSIGNED_BYTE;
        
    return result;    
  }

};

}

}

#endif

#ifndef LOST_BITMAP_IMAGELOADER_H
#define LOST_BITMAP_IMAGELOADER_H

#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "lost/bitmap/Bitmap.h"
#include "lost/common/Logger.h"
#include "lost/resource/Loader.h"
#include <sstream>
#include <stdexcept>
#include <map>

namespace lost
{

namespace bitmap
{

struct BitmapLoader
{
  boost::shared_ptr<resource::Loader> loader;

  typedef std::map<std::string, boost::shared_ptr<bitmap::Bitmap> > BitmapMap;

  BitmapLoader(boost::shared_ptr<resource::Loader> inLoader)
  : loader(inLoader)
  {
    DOUT("creating");
  }

  ~BitmapLoader()
  {
    DOUT("destroying");
  }  

  boost::shared_ptr<lost::bitmap::Bitmap> load(boost::filesystem::path inPath)
  {
    boost::shared_ptr<bitmap::Bitmap> result(new Bitmap);
    
    BitmapMap::iterator pos = location2bitmap.find(inPath.string());
    if(pos != location2bitmap.end())
    {
      result = pos->second;
    }
    else
    {
      boost::shared_ptr<resource::File> file = loader->load(inPath.string());
      result->init(file);
      location2bitmap[file->location] = result;
    }
    
    return result;
  }
  
  
  BitmapMap location2bitmap;
};
}
}

#endif

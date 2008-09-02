#include "lost/font/freetype/Library.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>

namespace lost
{
namespace font
{
namespace freetype
{

Library::Library()
{
  FT_Error error = FT_Init_FreeType(&mLibrary);
  if(error)
  {
    throw std::runtime_error("FT_Init_FreeType error: " + boost::lexical_cast<std::string>(error));
    return;
  }
}

Library::~Library()
{
  // get rid of fonts before the library handle gets invalid
  mFaceCache.clear(); 
  mFileCache.clear();
  FT_Error error = FT_Done_FreeType(mLibrary);
  // don't throw, just log
  if(error)
  {
    DOUT("FT_Done_FreeType error: "<< error);
  }
}

// FIXME: move caching into loader?
boost::shared_ptr<Face> Library::initFace(boost::shared_ptr<lost::resource::File> file)
{
  boost::shared_ptr<Face> result;
  FaceCache::iterator pos;
  pos = mFaceCache.find(file->location);
  if(pos != mFaceCache.end())
  {
    return pos->second;
  }
  else
  {
    FT_Face face;
    FT_Error error  = FT_New_Memory_Face(mLibrary, reinterpret_cast<FT_Byte*>(file->data.get()), file->size, 0, &face);
    if(error)
    {
      throw std::runtime_error("FT_New_Face error: "+boost::lexical_cast<std::string>(error));
    }
    result.reset(new Face(face));
    mFaceCache[file->location] = result;
    mFileCache[file->location] = file;
  }
  return result;
}


}
}
}
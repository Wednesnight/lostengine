#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "lost/font/freetype/Face.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace font
{
namespace freetype
{

Face::Face(boost::shared_ptr<Library> inLibrary,
           boost::shared_ptr<resource::File> inFile)
{
  FT_Error error  = FT_New_Memory_Face(inLibrary->library,
                                       reinterpret_cast<FT_Byte*>(inFile->data.get()),
                                       inFile->size,
                                       0,
                                       &mFace);
  if(error)
  {
    throw std::runtime_error("FT_New_Face error: "+boost::lexical_cast<std::string>(error));
  }

  mFile = inFile;
  mLibrary = inLibrary;
}

Face::~Face()
{
  FT_Error err = FT_Done_Face(mFace);
  if(err)
  {
    DOUT("FT_Done_Face error: "<<err);
  }
}


}
}
}

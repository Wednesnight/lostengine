#include "ft2build.h"
#include FT_FREETYPE_H
#include <stdexcept>
#include <sstream>
#include "lost/font/freetype/Face.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
#include "lost/font/freetype/Library.h"

namespace lost
{
namespace font
{
namespace freetype
{

using namespace std;

Face::Face(LibraryPtr inLibrary,
           common::DataPtr inData)
{
  FT_Error error  = FT_New_Memory_Face(inLibrary->library,
                                       reinterpret_cast<FT_Byte*>(inData->bytes.get()),
                                       inData->size,
                                       0,
                                       &mFace);
  if(error)
  {
    ostringstream os;
    os << "FT_New_Face error: " << error;
    throw std::runtime_error(os.str());
  }

  mFontData = inData;
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

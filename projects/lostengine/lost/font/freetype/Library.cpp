#include "ft2build.h"
#include FT_FREETYPE_H
#include "lost/font/freetype/Library.h"
#include "lost/common/Logger.h"
#include <stdexcept>
#include <sstream>

namespace lost
{
namespace font
{
namespace freetype
{

Library::Library()
{
  FT_Error error = FT_Init_FreeType(&library);
  if(error)
  {
    std::ostringstream os;
    os << "FT_Init_FreeType error: " << error;
    throw std::runtime_error(os.str());
    return;
  }
}

Library::~Library()
{
  FT_Error error = FT_Done_FreeType(library);
  // don't throw, just log
  if(error)
  {
    DOUT("FT_Done_FreeType error: "<< error);
  }
}
}
}
}

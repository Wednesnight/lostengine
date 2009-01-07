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
  FT_Error error = FT_Init_FreeType(&library);
  if(error)
  {
    throw std::runtime_error("FT_Init_FreeType error: " + boost::lexical_cast<std::string>(error));
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

#include "lost/font/freetype/Face.h"
#include "lost/common/Logger.h"

namespace lost
{
namespace font
{
namespace freetype
{

Face::Face(FT_Face inFace)
{
  mFace = inFace;
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
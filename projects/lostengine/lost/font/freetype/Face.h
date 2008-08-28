#ifndef LOST_FONT_FREETYPE_FACE_H
#define LOST_FONT_FREETYPE_FACE_H

#include "ft2build.h"
#include FT_FREETYPE_H


namespace lost
{
  namespace font
  {
    namespace freetype
    {
      struct Face
      {

        Face(FT_Face inFace);
        virtual ~Face();

        FT_Face face() { return mFace; }

      private:
        FT_Face     mFace;
      };
    }
  }
}

#endif

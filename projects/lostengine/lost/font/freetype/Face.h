#ifndef LOST_FONT_FREETYPE_FACE_H
#define LOST_FONT_FREETYPE_FACE_H

#include "lost/font/freetype/forward.h"
#include "lost/common/forward.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {    
      struct Face
      {
        Face(LibraryPtr inLibrary,
             common::DataPtr inData);
        virtual ~Face();

        FT_Face face() { return mFace; }

      private:
        FT_Face     mFace;
        
        // font faces are only valid as long as the loaded data stays alive
        // so every font face object takes care of its own data
        common::DataPtr mFontData;
        
        // the freetype Library must ony be destroyed after all fonts were removed.
        // in order to avoid crashes and keep the fonts alive as long as possible,
        // we keep a reference to the library inside the font.
        LibraryPtr mLibrary;
      };
    }
  }
}

#endif

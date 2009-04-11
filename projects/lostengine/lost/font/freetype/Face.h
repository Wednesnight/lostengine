#ifndef LOST_FONT_FREETYPE_FACE_H
#define LOST_FONT_FREETYPE_FACE_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include "lost/resource/File.h"
#include "lost/font/freetype/Library.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {
      struct Face;
      typedef boost::shared_ptr<Face> FacePtr;
    
      struct Face
      {
        Face(LibraryPtr inLibrary,
             resource::FilePtr inFile);
        virtual ~Face();

        FT_Face face() { return mFace; }

      private:
        FT_Face     mFace;
        
        // font faces are only valid as long as the loaded data stays alive
        // so every font face object takes care of its own data
        resource::FilePtr mFile;
        
        // the freetype Library must ony be destroyed after all fonts were removed.
        // in order to avoid crashes and keep the fonts alive as long as possible,
        // we keep a reference to the library inside the font.
        LibraryPtr mLibrary;
      };
    }
  }
}

#endif

#ifndef LOST_FONT_FREETYPE_FACE_H
#define LOST_FONT_FREETYPE_FACE_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include "lost/resource/File.h"
#include <boost/shared_ptr.hpp>

namespace lost
{
//  namespace resource { struct File; }
  
  namespace font
  {
    namespace freetype
    {
      struct Face
      {

        // FIXME: needs more configuration parameters for e.g. kerning or texture filtering
        Face(FT_Face inFace, boost::shared_ptr<lost::resource::File> inFile);
        virtual ~Face();

        FT_Face face() { return mFace; }

      private:
        FT_Face     mFace;
        // font faces are only valid as long as the loaded data stays alive
        // so every font face object takes care of its own data
        boost::shared_ptr<lost::resource::File> mFile; 
      };
    }
  }
}

#endif

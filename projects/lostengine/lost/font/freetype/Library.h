#ifndef LOST_FONT_FREETYPE_LIBRARY_H
#define LOST_FONT_FREETYPE_LIBRARY_H

#include "ft2build.h"
#include FT_FREETYPE_H

#include "lost/platform/shared_ptr.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {
      struct Library;
      typedef lost::shared_ptr<Library> LibraryPtr;
    
      // exactly one instance required for font creation.
      struct Library
      {
        Library();
        virtual ~Library();

        FT_Library    library;
      };
    }
  }
}


#endif

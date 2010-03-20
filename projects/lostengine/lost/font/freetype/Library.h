#ifndef LOST_FONT_FREETYPE_LIBRARY_H
#define LOST_FONT_FREETYPE_LIBRARY_H

#include "lost/font/freetype/forward.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {
    
      // exactly one instance required for font creation.
      struct Library
      {
        Library();
        virtual ~Library();

        static LibraryPtr create() { return LibraryPtr(new Library); }

        FT_Library    library;
      };
    }
  }
}


#endif

#ifndef LOST_FONT_FREETYPE_LIBRARY_H
#define LOST_FONT_FREETYPE_LIBRARY_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include <boost/shared_ptr.hpp>

namespace lost
{
  namespace font
  {
    namespace freetype
    {
      struct Library;
      typedef boost::shared_ptr<Library> LibraryPtr;
    
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

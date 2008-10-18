#ifndef LOST_FONT_FREETYPE_LIBRARY_H
#define LOST_FONT_FREETYPE_LIBRARY_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>
#include "lost/font/freetype/Face.h"
#include "lost/resource/File.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {
      /** initializes fonts via freetype2 from file or memory, and
       * caches them in a map.
       */
      struct Library
      {
        Library();
        virtual ~Library();

        /** creates a font from a file
         */
        boost::shared_ptr<Face> initFace(boost::shared_ptr<lost::resource::File> file);

      private:
        FT_Library    mLibrary;
      };
    }
  }
}


#endif

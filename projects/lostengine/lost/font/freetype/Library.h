#ifndef LOST_FONT_FREETYPE_LIBRARY_H
#define LOST_FONT_FREETYPE_LIBRARY_H

#include "ft2build.h"
#include FT_FREETYPE_H
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/font/freetype/Face.h"
#include <boost/lexical_cast.hpp>
#include "lost/resource/File.h"

namespace lost
{
  namespace font
  {
    namespace freetype
    {
      typedef std::map<std::string, boost::shared_ptr<Face> > FaceCache;
      /** loads fonts via freetype2, either from file or memory, and
       * caches them in a map.
       */
      struct Library
      {
        Library();
        virtual ~Library();

        /** loads a font from file via and caches it in the map using the path as key.
         */
        boost::shared_ptr<Face> initFace(boost::shared_ptr<lost::resource::File> file);

      private:
        FT_Library    mLibrary;
        FaceCache     mFaceCache;
      };
    }
  }
}


#endif

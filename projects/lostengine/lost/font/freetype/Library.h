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
        Library()
        {
          FT_Error error = FT_Init_FreeType(&mLibrary);
          if(error)
          {
            throw std::runtime_error("FT_Init_FreeType error: " + boost::lexical_cast<std::string>(error));
            return;
          }

        }
        virtual ~Library()
        {
          mFaceCache.clear(); // get rid of fonts before the library handle gets invalid
          FT_Error error = FT_Done_FreeType(mLibrary);
          if(error)
          {
            DOUT("FT_Done_FreeType error: "<< error);
          }
        }

        /** loads a font from file via freetype and caches it in the map using the path as key.
         */
        boost::shared_ptr<Face> initFaceFromFile(std::string path)
        {
          boost::shared_ptr<Face> result;
          FaceCache::iterator pos;
          pos = mFaceCache.find(path);
          if(pos != mFaceCache.end())
          {
            return pos->second;
          }
          else
          {
            FT_Face face;
            FT_Error error = FT_New_Face(mLibrary, path.c_str(), 0, &face);
            if(error)
            {
              throw std::runtime_error("FT_New_Face error: "+boost::lexical_cast<std::string>(error));
            }
            result.reset(new Face(face));
            mFaceCache[path] = result;
          }
          return result;
        }

      private:
        FT_Library    mLibrary;
        FaceCache     mFaceCache;
      };
    }
  }
}


#endif

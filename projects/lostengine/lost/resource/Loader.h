#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <map>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "lost/resource/Repository.h"
#include "lost/common/Logger.h"
#include "lost/font/TrueTypeFont.h"

namespace lost
{
  namespace resource
  {

    /** a simple caching resource loader for fonts and images.
     * Fonts and images are currently loaded as simple as possible.
     * Caching is implemented by using the file system paths as indices
     * in a map of shared_ptrs to these objects.
     * The only Fonts you can load through the ResourceLoader are truetype fonts.
     * If you need to use the builtin OpenGL fonts, you can simply instantiate
     *
     * FIXME: rewrite this so arbitrary sources for the loading process can be specified.
     *
     */
    struct Loader
    {
      Loader() {}
      virtual ~Loader() {}

      // FIXME: obsolete but still used in some testing code, remove asap
      boost::shared_ptr<lost::font::TrueTypeFont> loadFont(const std::string& pathToFont)
      {
        boost::shared_ptr<lost::font::TrueTypeFont> result;
        std::map<std::string, boost::shared_ptr<lost::font::TrueTypeFont> >::iterator pos;

        result.reset(new lost::font::TrueTypeFont(pathToFont));

        return result;
      }

      boost::shared_ptr<lost::resource::File> load( const std::string& inLocation)
      {
        boost::shared_ptr<lost::resource::File> result;

        // FIXME: determine failed loads by counting load attempts.
        //        if we finish the loops with zero successful loads, throw an exception.
        //        returned file size of zero is NOT a valid reason for throwing an exception.
        for (unsigned int idx = 0; idx < repositories.size(); ++idx)
        {
          try
          {
            result = repositories[idx]->load( inLocation);
            break;
          }
          catch (std::exception& e)
          {
            DOUT("trying next repository: " << e.what());
          }
        }

        DOUT("Loader: loaded file '" << result->location << "' [" << result->size << " Bytes]");
        return result;
      }

      void registerRepository( boost::shared_ptr<Repository> inRepository )
      {
        repositories.push_back( inRepository );
      }

    private:
      // resource repositories
      std::vector<boost::shared_ptr<Repository> > repositories;
    };

  }
}

#endif

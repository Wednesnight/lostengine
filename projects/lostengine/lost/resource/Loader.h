#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "lost/resource/Repository.h"
#include "lost/common/Logger.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {

    struct Loader
    {
      Loader() {}
      virtual ~Loader() {}

      boost::shared_ptr<lost::resource::File> load( const boost::filesystem::path& inPath)
      {
        boost::shared_ptr<lost::resource::File> result;

        // FIXME: determine failed loads by counting load attempts.
        //        if we finish the loops with zero successful loads, throw an exception.
        //        returned file size of zero is NOT a valid reason for throwing an exception.
        for (unsigned int idx = 0; idx < repositories.size(); ++idx)
        {
          try
          {
            result = repositories[idx]->load( inPath);
            break;
          }
          catch (std::exception& e)
          {
            DOUT("trying next repository: " << e.what());
          }
        }

        // we have to throw here, otherwise the following checks will segfault
        if(!result)
          throw std::runtime_error("couldn't load file: '"+inPath.string()+"'");
		  
        DOUT("Loader: loaded file '" << result->location << "' [" << result->size << " Bytes]");
        return result;
      }

      void addRepository( boost::shared_ptr<Repository> inRepository )
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

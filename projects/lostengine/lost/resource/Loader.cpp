#include "lost/resource/Loader.h"
#include "lost/resource/File.h"
#include "lost/resource/Repository.h"
#include "lost/common/Logger.h"
#include <stdexcept>

namespace lost
{
  namespace resource
  {

    Loader::Loader()
    {
    }

    Loader::~Loader()
    {
    }

    FilePtr Loader::load( const boost::filesystem::path& inPath)
    {
      FilePtr result;

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
//          DOUT("trying next repository: " << e.what());
        }
      }

      // we have to throw here, otherwise the following checks will segfault
      if(!result)
        throw LoaderError("couldn't load file: '"+inPath.string()+"'");

      DOUT("'"<<result->location << "' [" << result->size << " Bytes]");
      return result;
    }

    void Loader::addRepository( lost::shared_ptr<Repository> inRepository )
    {
      repositories.push_back( inRepository );
    }

  }
}

#include "lost/resource/Loader.h"
#include "lost/resource/Repository.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
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

    common::DataPtr Loader::load( const boost::filesystem::path& inPath)
    {
      common::DataPtr result;

      // FIXME: determine failed loads by counting load attempts.
      //        if we finish the loops with zero successful loads, throw an exception.
      //        returned file size of zero is NOT a valid reason for throwing an exception.
      for (unsigned int idx = 0; idx < repositories.size(); ++idx)
      {
          if(result = repositories[idx]->load( inPath))
            break;
      }

      // we have to throw here, otherwise the following checks will segfault
      if(!result)
        throw LoaderError(string("couldn't load file: '")+inPath.string().c_str()+"'");

//      DOUT("'"<<inPath << "' [" << result->size << " Bytes]");
      return result;
    }

    string Loader::locate(const boost::filesystem::path& inPath)
    {
      string result = inPath.string().c_str();
      for (unsigned int idx = 0; idx < repositories.size(); ++idx)
      {
        if(repositories[idx]->exists(result))
          break;
      }
      return result;
    }

    bool Loader::exists(const boost::filesystem::path& inPath)
    {
      bool result = false;
      string p = inPath.string().c_str();
      for (unsigned int idx = 0; idx < repositories.size(); ++idx)
      {
        if(repositories[idx]->exists(p))
        {
          result = true;
          break;
        }
      }
      
      return result;
    }

    void Loader::addRepository( lost::shared_ptr<Repository> inRepository )
    {
      repositories.push_back( inRepository );
    }

  }
}

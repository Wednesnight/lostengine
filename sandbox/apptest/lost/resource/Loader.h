#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include "EASTL/vector.h"
#include <stdexcept>
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {
    struct LoaderError : public std::runtime_error { LoaderError(const eastl::string& error) : runtime_error(error.c_str()) {} };

    struct Loader
    {
      Loader();
      virtual ~Loader();

      common::DataPtr load( const boost::filesystem::path& inPath);
      eastl::string locate(const boost::filesystem::path& inPath);
      bool exists(const boost::filesystem::path& inPath);
      void addRepository( RepositoryPtr inRepository );


      static LoaderPtr create() { return LoaderPtr(new Loader); }
    private:
      // resource repositories
      eastl::vector<RepositoryPtr> repositories;
    };

  }
}

#endif

#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <vector>
#include <stdexcept>
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {
    struct LoaderError : public std::runtime_error { LoaderError(const std::string& error) : runtime_error(error) {} };

    struct Loader
    {
      Loader();
      virtual ~Loader();

      common::DataPtr load( const boost::filesystem::path& inPath);
      std::string locate(const boost::filesystem::path& inPath);
      bool exists(const boost::filesystem::path& inPath);
      void addRepository( RepositoryPtr inRepository );


      static LoaderPtr create() { return LoaderPtr(new Loader); }
    private:
      // resource repositories
      std::vector<RepositoryPtr> repositories;
    };

  }
}

#endif

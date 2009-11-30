#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <vector>
#include "lost/resource/Repository.h"
#include <stdexcept>

namespace lost
{
  namespace resource
  {
    struct Loader;
    typedef lost::shared_ptr<Loader> LoaderPtr;

    struct LoaderError : public std::runtime_error { LoaderError(const std::string& error) : runtime_error(error) {} };

    struct Loader
    {
      Loader();
      virtual ~Loader();

      common::DataPtr load( const boost::filesystem::path& inPath);
      void addRepository( RepositoryPtr inRepository );


      static LoaderPtr create() { return LoaderPtr(new Loader); }
    private:
      // resource repositories
      std::vector<RepositoryPtr> repositories;
    };

  }
}

#endif

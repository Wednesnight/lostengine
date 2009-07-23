#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <vector>
#include "lost/resource/Repository.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {
    struct Loader;
    typedef lost::shared_ptr<Loader> LoaderPtr;

    struct File;
    typedef lost::shared_ptr<File> FilePtr;

    struct Repository;
    typedef lost::shared_ptr<Repository> RepositoryPtr;

    struct LoaderError : public std::runtime_error { LoaderError(const std::string& error) : runtime_error(error) {} };

    struct Loader
    {
      Loader();
      virtual ~Loader();

      FilePtr load( const boost::filesystem::path& inPath);
      void addRepository( RepositoryPtr inRepository );

    private:
      // resource repositories
      std::vector<RepositoryPtr> repositories;
    };

  }
}

#endif

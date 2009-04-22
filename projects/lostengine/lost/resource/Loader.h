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

    struct Loader
    {
      Loader();
      virtual ~Loader();

      FilePtr load( const boost::filesystem::path& inPath);
      void addRepository( lost::shared_ptr<Repository> inRepository );

    private:
      // resource repositories
      std::vector<lost::shared_ptr<Repository> > repositories;
    };

  }
}

#endif

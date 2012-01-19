#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <stdexcept>
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include "lost/fs/Path.h"

namespace lost
{
  namespace resource
  {
    struct LoaderError : public std::runtime_error { LoaderError(const string& error) : runtime_error(error.c_str()) {} };

    struct Loader
    {
      Loader();
      virtual ~Loader();

      common::DataPtr load( const lost::fs::Path& inPath);
      string locate(const lost::fs::Path& inPath);
      bool exists(const lost::fs::Path& inPath);
      void addRepository( RepositoryPtr inRepository );


      static LoaderPtr create() { return LoaderPtr(new Loader); }
    private:
      // resource repositories
      vector<RepositoryPtr> repositories;
    };

  }
}

#endif

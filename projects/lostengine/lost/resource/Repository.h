#ifndef LOST_RESOURCE_REPOSITORY_H
#define LOST_RESOURCE_REPOSITORY_H

#include "lost/platform/shared_ptr.h"

// filesystem path foward declaration
namespace boost
{
namespace filesystem
{
    template<typename S, typename PT> struct basic_path;
    struct path_traits;
    typedef basic_path< std::string, path_traits > path;
}
}

namespace lost
{
  namespace resource
  {
  struct File;
  typedef lost::shared_ptr<File> FilePtr;

    struct Repository;
    typedef lost::shared_ptr<Repository> RepositoryPtr;

    struct Repository
    {
      Repository() {}
      virtual ~Repository() {}

      virtual FilePtr load( const boost::filesystem::path& inPath) = 0;
    };
  }
}

#endif

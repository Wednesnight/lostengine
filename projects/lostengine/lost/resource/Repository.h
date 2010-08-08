#ifndef LOST_RESOURCE_REPOSITORY_H
#define LOST_RESOURCE_REPOSITORY_H

#include "lost/platform/shared_ptr.h"
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include <string>

// filesystem path foward declaration
namespace boost
{
namespace filesystem
{
    template<typename S, typename PT> class basic_path;
    struct path_traits;
    typedef basic_path< std::string, path_traits > path;
}
}

namespace lost
{
  namespace resource
  {
    struct Repository
    {
      Repository() {}
      virtual ~Repository() {}

      virtual common::DataPtr load( const boost::filesystem::path& inPath) = 0;
      virtual bool exists(std::string& inOutRelativePath) = 0;
    };
  }
}

#endif

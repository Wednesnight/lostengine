#ifndef LOST_RESOURCE_REPOSITORY_H
#define LOST_RESOURCE_REPOSITORY_H

#include "lost/resource/File.h"
#include <boost/filesystem.hpp>

namespace lost
{
  namespace resource
  {

    struct Repository
    {
      Repository() {}
      virtual ~Repository() {}

      virtual boost::shared_ptr<lost::resource::File> load( const boost::filesystem::path& inPath) = 0;
    };
  }
}

#endif

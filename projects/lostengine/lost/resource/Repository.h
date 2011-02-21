#ifndef LOST_RESOURCE_REPOSITORY_H
#define LOST_RESOURCE_REPOSITORY_H

#include "lost/platform/shared_ptr.h"
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include <string>
#include <boost/filesystem.hpp>


namespace lost
{
  namespace resource
  {
    struct Repository
    {
      Repository() {}
      virtual ~Repository() {}

      // read
      virtual common::DataPtr load( const boost::filesystem::path& inPath) = 0;
      virtual bool exists(std::string& inOutRelativePath) = 0;

      // write
      virtual void write(const boost::filesystem::path& inPath, const common::DataPtr& inData) = 0;
    };
  }
}

#endif

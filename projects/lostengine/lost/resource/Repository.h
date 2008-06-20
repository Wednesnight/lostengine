#ifndef LOST_RESOURCE_REPOSITORY_H
#define LOST_RESOURCE_REPOSITORY_H

#include <string>
#include "lost/common/Config.h"
#include <boost/shared_ptr.hpp>
#include "lost/resource/File.h"

namespace lost
{
  namespace resource
  {

    struct Repository
    {
      boost::shared_ptr<common::Config> config;

      Repository( boost::shared_ptr<common::Config> inConfig ) : config(inConfig) {}
      virtual ~Repository() {}

      virtual boost::shared_ptr<lost::resource::File> load( const std::string& inPath) = 0;

    };

  }
}

#endif

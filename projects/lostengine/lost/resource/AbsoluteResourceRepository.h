#ifndef LOST_RESOURCE_ABSOLUTERESOURCEREPOSITORY_H
#define LOST_RESOURCE_ABSOLUTERESOURCEREPOSITORY_H

#include <boost/filesystem.hpp>
#include "lost/resource/Repository.h"

namespace lost
{
  namespace resource
  {
    
    struct File;
    typedef lost::shared_ptr<File> FilePtr;
    
    struct AbsoluteResourceRepository : public Repository
    {
      AbsoluteResourceRepository();
      virtual ~AbsoluteResourceRepository();
      
      virtual FilePtr load( const boost::filesystem::path& absolutePath);
    };
  }
}

#endif

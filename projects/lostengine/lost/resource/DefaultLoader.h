#ifndef LOST_RESOURCE_DEFAULTLOADER_H
#define LOST_RESOURCE_DEFAULTLOADER_H

#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/resource/Loader.h"

namespace lost
{
namespace resource
{
  struct DefaultLoader : public Loader
{
  DefaultLoader()
  {
    addRepository(boost::shared_ptr<Repository>(new ApplicationResourceRepository));
  }
  
  virtual ~DefaultLoader() {}
};
}  
}

#endif
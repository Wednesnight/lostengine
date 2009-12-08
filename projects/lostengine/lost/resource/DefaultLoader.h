#ifndef LOST_RESOURCE_DEFAULTLOADER_H
#define LOST_RESOURCE_DEFAULTLOADER_H

#include "lost/resource/Loader.h"

namespace lost
{
  namespace resource
  {
    struct DefaultLoader;
    typedef lost::shared_ptr<DefaultLoader> DefaultLoaderPtr;
  
    struct DefaultLoader : public Loader
    {
      DefaultLoader();
      virtual ~DefaultLoader();
      
      static DefaultLoaderPtr create() { return DefaultLoaderPtr(new DefaultLoader()); }
    };
  }
}

#endif

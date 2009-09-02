#ifndef LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H
#define LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H

#include "lost/resource/FilesystemRepository.h"

namespace lost
{
  namespace resource
  {
    struct ApplicationResourceRepository;
    typedef lost::shared_ptr<ApplicationResourceRepository> ApplicationResourceRepositoryPtr;

    struct ApplicationResourceRepository : public FilesystemRepository
    {
      ApplicationResourceRepository();
      static ApplicationResourceRepositoryPtr create();
    };
  }
}

#endif

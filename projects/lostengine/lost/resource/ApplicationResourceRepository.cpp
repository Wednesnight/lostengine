#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/platform/Platform.h"
#include "lost/resource/Helper.h"

namespace lost
{
  namespace resource
  {
    ApplicationResourceRepository::ApplicationResourceRepository()
    : FilesystemRepository(platform::getResourcePath())
    {
    }

    ApplicationResourceRepositoryPtr ApplicationResourceRepository::create()
    {
      return ApplicationResourceRepositoryPtr(new ApplicationResourceRepository());
    }
    
  }
}

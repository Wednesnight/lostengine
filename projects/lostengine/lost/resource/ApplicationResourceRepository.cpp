#include <boost/filesystem.hpp>
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

    RepositoryPtr ApplicationResourceRepository::create()
    {
      return ApplicationResourceRepositoryPtr(new ApplicationResourceRepository());
    }
    
  }
}

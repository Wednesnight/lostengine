#include "lost/resource/Bundle.h"
#include "lost/resource/FilesystemRepository.h"

namespace lost
{
namespace resource
{

Bundle::Bundle(const fs::Path& inRootDir)  
{
  RepositoryPtr repo = FilesystemRepository::create(inRootDir);
  addRepository(repo);
}

BundlePtr Bundle::create(const fs::Path& inRootDir)
{
  return BundlePtr(new Bundle(inRootDir));
}

}
}
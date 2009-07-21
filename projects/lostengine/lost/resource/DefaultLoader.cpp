#include "lost/resource/DefaultLoader.h"
#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/resource/AbsoluteResourceRepository.h"

namespace lost
{
namespace resource
{
DefaultLoader::DefaultLoader()
{
  addRepository(lost::shared_ptr<Repository>(new ApplicationResourceRepository));
  addRepository(lost::shared_ptr<Repository>(new AbsoluteResourceRepository));
}

DefaultLoader::~DefaultLoader() {}
}
}

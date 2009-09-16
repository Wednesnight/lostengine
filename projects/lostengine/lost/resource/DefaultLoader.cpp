#include <boost/filesystem.hpp>
#include "lost/resource/DefaultLoader.h"
#include "lost/resource/ApplicationResourceRepository.h"

namespace lost
{
namespace resource
{
DefaultLoader::DefaultLoader()
{
  addRepository(lost::shared_ptr<Repository>(new ApplicationResourceRepository));
}

DefaultLoader::~DefaultLoader() {}
}
}

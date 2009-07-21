#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include "lost/resource/Helper.h"
#include <fstream>

namespace lost
{
namespace resource
{
ApplicationResourceRepository::ApplicationResourceRepository()
{
  applicationResourcePath = platform::getResourcePath();
}

ApplicationResourceRepository::~ApplicationResourceRepository() {}

lost::shared_ptr<lost::resource::File> ApplicationResourceRepository::load( const boost::filesystem::path& relativePath)
{
  boost::filesystem::path absolutePath = applicationResourcePath / relativePath;
  FilePtr result = loadFromAbsolutePath(absolutePath.string());
  result->location = relativePath.string();
  
  return result;
}
}
}

#include "lost/resource/ApplicationResourceRepository.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
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
  lost::shared_ptr<lost::resource::File> result(new lost::resource::File);
  boost::filesystem::path absolutePath = applicationResourcePath / relativePath;
  std::string path = absolutePath.string();
  result->location = relativePath.string();

  std::ifstream ifs( path.c_str(), std::ios_base::in | std::ios_base::binary );

  if(!ifs.good()) { throw std::runtime_error( "couldn't open file: '"+ path +"'" ); }

  ifs.seekg(0,std::ios_base::end);
  if(ifs.fail()) throw std::runtime_error("failed");
  unsigned long filesize = ifs.tellg();
  ifs.seekg(std::ios_base::beg);
  result->size = filesize;
  result->data.reset(new char[filesize]);
  ifs.read(result->data.get(), filesize);

  return result;
}
}
}

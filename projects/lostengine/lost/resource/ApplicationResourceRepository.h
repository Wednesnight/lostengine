#ifndef LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H
#define LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H

#include "lost/resource/Repository.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include <fstream>

namespace lost
{
namespace resource
{
struct ApplicationResourceRepository : public Repository
{
  boost::filesystem::path applicationResourcePath;

  ApplicationResourceRepository()
  {
    applicationResourcePath = platform::getResourcePath();
  }

  virtual ~ApplicationResourceRepository() {}

  virtual boost::shared_ptr<lost::resource::File> load( const boost::filesystem::path& relativePath)
  {
    boost::shared_ptr<lost::resource::File> result(new lost::resource::File);
    boost::filesystem::path absolutePath = applicationResourcePath / relativePath;
    std::string path = absolutePath.string();
//    DOUT("loading: " << path);
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
};
}
}

#endif

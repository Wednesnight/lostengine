#ifndef LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H
#define LOST_RESOURCE_APPLICATIONRESOURCEREPOSITORY_H

#include <boost/filesystem.hpp>
#include "lost/resource/Repository.h"

namespace lost
{
namespace resource
{

struct File;
typedef lost::shared_ptr<File> FilePtr;

struct ApplicationResourceRepository : public Repository
{
  boost::filesystem::path applicationResourcePath;

  ApplicationResourceRepository();
  virtual ~ApplicationResourceRepository();

  virtual FilePtr load( const boost::filesystem::path& relativePath);
};
}
}

#endif

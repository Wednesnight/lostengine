#ifndef LOST_RESOURCE_FILESYSTEMREPOSITORY_H
#define LOST_RESOURCE_FILESYSTEMREPOSITORY_H

#include "lost/resource/Repository.h"
#include <boost/filesystem.hpp>

namespace lost
{
namespace resource
{
struct FilesystemRepository;
typedef lost::shared_ptr<FilesystemRepository> FilesystemRepositoryPtr;

struct FilesystemRepository : Repository
{
  boost::filesystem::path rootDirectory;

  FilesystemRepository(const boost::filesystem::path& inRootDir);
  virtual ~FilesystemRepository() {};

  virtual FilePtr load( const boost::filesystem::path& relativePath);
  
};
}
}

#endif
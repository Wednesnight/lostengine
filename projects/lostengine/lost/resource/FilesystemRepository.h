#ifndef LOST_RESOURCE_FILESYSTEMREPOSITORY_H
#define LOST_RESOURCE_FILESYSTEMREPOSITORY_H

#include "lost/resource/Repository.h"

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

  virtual common::DataPtr load(const boost::filesystem::path& inRootDir);
  static FilesystemRepositoryPtr create(const boost::filesystem::path& inRootDir);
  
};
}
}

#endif
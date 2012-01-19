#ifndef LOST_RESOURCE_FILESYSTEMREPOSITORY_H
#define LOST_RESOURCE_FILESYSTEMREPOSITORY_H

#include "lost/resource/Repository.h"

namespace lost
{
namespace resource
{
struct FilesystemRepository : Repository
{
  string rootDirectory;

  FilesystemRepository(const lost::fs::Path& inRootDir);
  virtual ~FilesystemRepository() {};

  virtual common::DataPtr load(const lost::fs::Path& inRootDir);
  virtual bool exists(string& path);
  static RepositoryPtr create(const string& inRootDir);
  
};
}
}

#endif

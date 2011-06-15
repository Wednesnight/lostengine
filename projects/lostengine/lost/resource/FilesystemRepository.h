#ifndef LOST_RESOURCE_FILESYSTEMREPOSITORY_H
#define LOST_RESOURCE_FILESYSTEMREPOSITORY_H

#include "lost/resource/Repository.h"
#include <boost/filesystem.hpp>

namespace lost
{
namespace resource
{
struct FilesystemRepository : Repository
{
  string rootDirectory;

  FilesystemRepository(const boost::filesystem::path& inRootDir);
  virtual ~FilesystemRepository() {};

  virtual common::DataPtr load(const boost::filesystem::path& inRootDir);
  virtual bool exists(string& path);
  static RepositoryPtr create(const string& inRootDir);
  
};
}
}

#endif

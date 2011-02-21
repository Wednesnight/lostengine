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
  std::string rootDirectory;

  FilesystemRepository(const boost::filesystem::path& inRootDir);
  virtual ~FilesystemRepository() {};

  virtual common::DataPtr load(const boost::filesystem::path& inRootDir);
  virtual bool exists(std::string& path);
  virtual void write(const boost::filesystem::path& inPath, const common::DataPtr& inData);
  static RepositoryPtr create(const std::string& inRootDir);
//  static FilesystemRepositoryPtr create(const boost::filesystem::path& inRootDir);
  
};
}
}

#endif

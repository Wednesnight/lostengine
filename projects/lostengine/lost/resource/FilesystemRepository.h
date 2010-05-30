#ifndef LOST_RESOURCE_FILESYSTEMREPOSITORY_H
#define LOST_RESOURCE_FILESYSTEMREPOSITORY_H

#include "lost/resource/Repository.h"
#include <boost/filesystem/path.hpp>

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
  virtual bool locate(std::string& inRootDir);
  static RepositoryPtr create(const std::string& inRootDir);
//  static FilesystemRepositoryPtr create(const boost::filesystem::path& inRootDir);
  
};
}
}

#endif
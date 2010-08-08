#ifndef LOST_RESOURCE_FILESYSTEMREPOSITORY_H
#define LOST_RESOURCE_FILESYSTEMREPOSITORY_H

#include "lost/resource/Repository.h"

// filesystem path foward declaration
namespace boost
{
namespace filesystem
{
    template<typename S, typename PT> class basic_path;
    struct path_traits;
    typedef basic_path< std::string, path_traits > path;
}
}

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
  static RepositoryPtr create(const std::string& inRootDir);
//  static FilesystemRepositoryPtr create(const boost::filesystem::path& inRootDir);
  
};
}
}

#endif
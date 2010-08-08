#include <boost/filesystem.hpp>
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/Helper.h"

namespace lost
{
namespace resource
{

using namespace boost::filesystem;

FilesystemRepository::FilesystemRepository(const boost::filesystem::path& inRootDir)
:rootDirectory(inRootDir.string())
{
}

common::DataPtr FilesystemRepository::load( const boost::filesystem::path& relativePath)
{
  boost::filesystem::path absolutePath = path(rootDirectory) / relativePath;
  return loadFromAbsolutePath(absolutePath.string());;
}

bool FilesystemRepository::exists(std::string& inOutRelativePath)
{
  boost::filesystem::path absolutePath = path(rootDirectory) / inOutRelativePath;
  bool exists = fileExists(absolutePath.string());
  if (exists) {
    inOutRelativePath = absolutePath.string();
  }
  return exists;
}

RepositoryPtr FilesystemRepository::create(const std::string& inRootDir)
{
  return FilesystemRepositoryPtr(new FilesystemRepository(inRootDir));
}

}
}
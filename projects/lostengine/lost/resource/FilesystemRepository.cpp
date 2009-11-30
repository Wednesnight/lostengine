#include <boost/filesystem.hpp>
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/Helper.h"

namespace lost
{
namespace resource
{

FilesystemRepository::FilesystemRepository(const boost::filesystem::path& inRootDir)
:rootDirectory(inRootDir)
{
}

common::DataPtr FilesystemRepository::load( const boost::filesystem::path& relativePath)
{
  boost::filesystem::path absolutePath = rootDirectory / relativePath;
  return loadFromAbsolutePath(absolutePath.string());;
}

FilesystemRepositoryPtr FilesystemRepository::create(const boost::filesystem::path& inRootDir)
{
  return FilesystemRepositoryPtr(new FilesystemRepository(inRootDir));
}

}
}
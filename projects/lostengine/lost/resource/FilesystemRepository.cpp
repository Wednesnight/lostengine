#include <boost/filesystem.hpp>
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/Helper.h"
#include "lost/resource/File.h"

namespace lost
{
namespace resource
{

FilesystemRepository::FilesystemRepository(const boost::filesystem::path& inRootDir)
:rootDirectory(inRootDir)
{
}

FilePtr FilesystemRepository::load( const boost::filesystem::path& relativePath)
{
  boost::filesystem::path absolutePath = rootDirectory / relativePath;
  FilePtr result = loadFromAbsolutePath(absolutePath.string());
  return result;
}

FilesystemRepositoryPtr FilesystemRepository::create(const boost::filesystem::path& inRootDir)
{
  return FilesystemRepositoryPtr(new FilesystemRepository(inRootDir));
}

}
}
#include <boost/filesystem.hpp>
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/Helper.h"

namespace lost
{
  namespace resource
  {

    using namespace boost::filesystem;

    FilesystemRepository::FilesystemRepository(const boost::filesystem::path& inRootDir)
    :rootDirectory(inRootDir.string().c_str())
    {
    }

    common::DataPtr FilesystemRepository::load( const boost::filesystem::path& relativePath)
    {
      boost::filesystem::path absolutePath = path(rootDirectory) / relativePath;
      return loadFromAbsolutePath(absolutePath.string().c_str());;
    }

    bool FilesystemRepository::exists(string& inOutRelativePath)
    {
      boost::filesystem::path absolutePath = path(rootDirectory) / inOutRelativePath;
      bool exists = fileExists(absolutePath.string().c_str());
      if (exists) {
        inOutRelativePath = absolutePath.string().c_str();
      }
      return exists;
    }

    RepositoryPtr FilesystemRepository::create(const string& inRootDir)
    {
      return FilesystemRepositoryPtr(new FilesystemRepository(inRootDir));
    }

  }
}

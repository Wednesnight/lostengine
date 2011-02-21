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

    void FilesystemRepository::write(const boost::filesystem::path& inPath, const common::DataPtr& inData)
    {
      boost::filesystem::path absolutePath = path(rootDirectory) / inPath;
      writeToAbsolutePath(absolutePath.string(), inData);
    }

    RepositoryPtr FilesystemRepository::create(const std::string& inRootDir)
    {
      return FilesystemRepositoryPtr(new FilesystemRepository(inRootDir));
    }

  }
}

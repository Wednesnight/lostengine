#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/Helper.h"
//#include "lost/fs/Path.h"
namespace lost
{
  namespace resource
  {
    FilesystemRepository::FilesystemRepository(const lost::fs::Path& inRootDir)
    :rootDirectory(inRootDir.string().c_str())
    {
    }

    common::DataPtr FilesystemRepository::load( const lost::fs::Path& relativePath)
    {
      lost::fs::Path absolutePath = lost::fs::Path(rootDirectory) / relativePath;
      return loadFromAbsolutePath(absolutePath.string().c_str());;
    }

    bool FilesystemRepository::exists(string& inOutRelativePath)
    {
      lost::fs::Path absolutePath = lost::fs::Path(rootDirectory) / inOutRelativePath;
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

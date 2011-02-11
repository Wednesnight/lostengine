#ifndef LOST_RESOURCE_ANDROIDASSETREPOSITORY_H
#define LOST_RESOURCE_ANDROIDASSETREPOSITORY_H

#include "lost/resource/forward.h"
#include "lost/resource/Repository.h"

#include <boost/filesystem.hpp>
#include <zzip/zzip.h>

extern "C" {
  #include <sys/types.h>
  #include <android/asset_manager.h>
  #include <android/asset_manager_jni.h>
}

namespace lost
{
  namespace resource
  {
    struct AndroidAssetRepository;
    typedef lost::shared_ptr<AndroidAssetRepository> AndroidAssetRepositoryPtr;

    struct AndroidAssetRepository : public Repository
    {
      boost::filesystem::path subdir;
      AAssetManager* assetManager;
      ZZIP_DIR* apk;
      
      AndroidAssetRepository(AAssetManager* assetManager, const boost::filesystem::path& subdir = "");
      virtual ~AndroidAssetRepository();
      
      common::DataPtr load(const boost::filesystem::path& filename);
      bool exists(std::string& path);
      static RepositoryPtr create(AAssetManager* assetManager, const boost::filesystem::path& subdir = "");
    };
  }
}

#endif

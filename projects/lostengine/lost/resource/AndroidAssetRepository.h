#ifndef LOST_RESOURCE_ANDROIDASSETREPOSITORY_H
#define LOST_RESOURCE_ANDROIDASSETREPOSITORY_H

#include "lost/resource/forward.h"
#include "lost/resource/Repository.h"

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
      lost::fs::Path subdir;
      AAssetManager* assetManager;
      ZZIP_DIR* apk;
      
      AndroidAssetRepository(AAssetManager* assetManager, const lost::fs::Path& subdir = "");
      virtual ~AndroidAssetRepository();
      
      common::DataPtr load(const lost::fs::Path& filename);
      bool exists(string& path);
      static RepositoryPtr create(AAssetManager* assetManager, const lost::fs::Path& subdir = "");
    };
  }
}

#endif

/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/resource/AndroidAssetRepository.h"
#include "lost/common/Data.h"
#include "lost/common/Logger.h"

#include <stdio.h>
#include <sys/stat.h>

namespace lost
{
  namespace resource
  {
    
    AndroidAssetRepository::AndroidAssetRepository(AAssetManager* assetManager, const lost::fs::Path& subdir)
    : assetManager(assetManager),
      subdir(subdir),
      apk(NULL)
    {
    }

    AndroidAssetRepository::~AndroidAssetRepository() {
      if (apk != NULL) {
        zzip_dir_close(apk);
      }
    }

    common::DataPtr AndroidAssetRepository::load(const lost::fs::Path& filename)
    {
      common::DataPtr result;
      lost::fs::Path f = subdir / filename;

      DOUT("loading: " << filename);
      
      if (apk == NULL) {
        AAsset* asset = AAssetManager_open(assetManager, f.string().c_str(), AASSET_MODE_UNKNOWN);
        if (asset == NULL) {
          asset = AAssetManager_open(assetManager, filename.string().c_str(), AASSET_MODE_UNKNOWN);
        }
        if (asset != NULL) {
          DOUT("reading asset <" << f << ">");
          off_t s, size;
          int fd = AAsset_openFileDescriptor(asset, &s, &size);
          if (fd > -1) {
            DOUT("asset offset: " << s);
            DOUT("asset size: " << size);
            zzip_error_t error;
            apk = zzip_dir_fdopen(dup(fd), &error);
            if (apk == NULL) {
              WOUT("couldn't open zip file: error " << error);
            }
          }
          else {
            WOUT("couldn't get file descriptor");
          }
          AAsset_close(asset);
        }
      }

      if (apk != NULL) {
        ZZIP_FILE* file = zzip_file_open(apk, ("assets" / f).string().c_str(), O_RDONLY);
        if (file == NULL) {
          file = zzip_file_open(apk, ("assets" / filename).string().c_str(), O_RDONLY);
        }
        if (file != NULL) {
          ZZIP_STAT stat;
          if (zzip_fstat(file, &stat) > -1) {
            DOUT("file size: " << stat.st_size);
            result.reset(new common::Data);
            result->size = stat.st_size;
            result->bytes.reset(new char[stat.st_size]);
            zzip_file_read(file, result->bytes.get(), stat.st_size);
          }
          else {
            WOUT("couldn't get zip file size");
          }
          zzip_file_close(file);
        }
        else {
          WOUT("couldn't open zip file");
        }
      }

      return result;
    }
    
    bool AndroidAssetRepository::exists(string& filename)
    {
      bool result = false;
      lost::fs::Path f = subdir / filename;
      AAsset* asset = AAssetManager_open(assetManager, f.string().c_str(), AASSET_MODE_UNKNOWN);
      if (asset != NULL) {
        result = true;
        AAsset_close(asset);
      }
      return result;
    }
    
    RepositoryPtr AndroidAssetRepository::create(AAssetManager* assetManager, const lost::fs::path& subdir)
    {
      return AndroidAssetRepositoryPtr(new AndroidAssetRepository(assetManager, subdir));
    }
    
  }
}

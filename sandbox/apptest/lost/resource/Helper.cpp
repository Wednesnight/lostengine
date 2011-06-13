#include <stdio.h>

#include "lost/common/Data.h"
#include "lost/resource/Helper.h"
#include "lost/common/Logger.h"
#include "zzip/zzip.h"

namespace lost
{
  namespace resource
  {

    common::DataPtr loadFromAbsolutePath(const char* path)
    {
      common::DataPtr result;

      ZZIP_FILE* file = zzip_fopen(path, "rb");
      if (file != NULL)
      {
        if (zzip_file_real(file)) { /*DOUT("read from filesystem <" << inPath << ">");*/ }
          else { /*DOUT("read from zip <" << inPath << ">");*/ }

        ZZIP_STAT stat;
        if (zzip_fstat(file, &stat) > -1)
        {
          result.reset(new common::Data);
          result->size = stat.st_size;
          result->bytes.reset(new char[stat.st_size]);
          zzip_fread(result->bytes.get(), 1, stat.st_size, file);
        }
        zzip_fclose(file);
      }
      
      return result;
    }

    bool fileExists(const char* path)
    {
      ZZIP_FILE* file = zzip_fopen(path, "rb");
      bool result = (file != NULL);
      if (result)
      {
        zzip_fclose(file);
      }
      return result;
    }
  }
}

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

#include <stdio.h>

#include "lost/common/Data.h"
#include "lost/resource/Helper.h"
#include "lost/common/Logger.h"
#include "zzip/zzip.h"

namespace lost
{
  namespace resource
  {

    common::DataPtr loadFromAbsolutePath(const string& inPath)
    {
      common::DataPtr result;

      ZZIP_FILE* file = zzip_fopen(inPath.c_str(), "rb");
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

    bool fileExists(const string& inPath)
    {
      ZZIP_FILE* file = zzip_fopen(inPath.c_str(), "rb");
      bool result = (file != NULL);
      if (result)
      {
        zzip_fclose(file);
      }
      return result;
    }

    void writeToAbsolutePath(const string& inPath, const common::DataPtr& inData)
    {
      FILE* file = fopen(inPath.c_str(), "wb");
      if (file != NULL) {
        if (fwrite(inData->bytes.get(), 1, inData->size, file) != inData->size) {
          // raise error
        }
        fclose(file);
      }
      else {
        // raise error
      }
    }

  }
}

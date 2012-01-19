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

#include "lost/resource/Loader.h"
#include "lost/resource/Repository.h"
#include "lost/common/Logger.h"
#include "lost/common/Data.h"
#include <stdexcept>

namespace lost
{
  namespace resource
  {

    Loader::Loader()
    {
    }

    Loader::~Loader()
    {
    }

    common::DataPtr Loader::load( const lost::fs::Path& inPath)
    {
      common::DataPtr result;

      // FIXME: determine failed loads by counting load attempts.
      //        if we finish the loops with zero successful loads, throw an exception.
      //        returned file size of zero is NOT a valid reason for throwing an exception.
      for (unsigned int idx = 0; idx < repositories.size(); ++idx)
      {
          if((result = repositories[idx]->load( inPath)))
            break;
      }

      // we have to throw here, otherwise the following checks will segfault
      if(!result)
        throw LoaderError(string("couldn't load file: '")+inPath.string().c_str()+"'");

//      DOUT("'"<<inPath << "' [" << result->size << " Bytes]");
      return result;
    }

    string Loader::locate(const lost::fs::Path& inPath)
    {
      string result = inPath.string().c_str();
      for (unsigned int idx = 0; idx < repositories.size(); ++idx)
      {
        if(repositories[idx]->exists(result))
          break;
      }
      return result;
    }

    bool Loader::exists(const lost::fs::Path& inPath)
    {
      bool result = false;
      string p = inPath.string().c_str();
      for (unsigned int idx = 0; idx < repositories.size(); ++idx)
      {
        if(repositories[idx]->exists(p))
        {
          result = true;
          break;
        }
      }
      
      return result;
    }

    void Loader::addRepository( lost::shared_ptr<Repository> inRepository )
    {
      repositories.push_back( inRepository );
    }

  }
}

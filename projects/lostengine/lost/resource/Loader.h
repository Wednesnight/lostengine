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

#ifndef LOST_RESOURCE_LOADER_H
#define LOST_RESOURCE_LOADER_H

#include <stdexcept>
#include "lost/common/forward.h"
#include "lost/resource/forward.h"
#include "lost/fs/Path.h"

namespace lost
{
  namespace resource
  {
    struct LoaderError : public std::runtime_error { LoaderError(const string& error) : runtime_error(error.c_str()) {} };

    struct Loader
    {
      Loader();
      virtual ~Loader();

      common::DataPtr load( const lost::fs::Path& inPath);
      string locate(const lost::fs::Path& inPath);
      bool exists(const lost::fs::Path& inPath);
      void addRepository( RepositoryPtr inRepository );


      static LoaderPtr create() { return LoaderPtr(new Loader); }
    private:
      // resource repositories
      vector<RepositoryPtr> repositories;
    };

  }
}

#endif

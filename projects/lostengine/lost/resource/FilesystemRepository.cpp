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

#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/Helper.h"
//#include "lost/fs/Path.h"
namespace lost
{
  namespace resource
  {
    FilesystemRepository::FilesystemRepository(const lost::fs::Path& inRootDir)
    :rootDirectory(inRootDir)
    {
    }

    common::DataPtr FilesystemRepository::load( const lost::fs::Path& relativePath)
    {
      lost::fs::Path absolutePath = rootDirectory / relativePath;
      return loadFromAbsolutePath(absolutePath.string().c_str());
    }

    bool FilesystemRepository::exists(string& inOutRelativePath)
    {
      lost::fs::Path absolutePath = rootDirectory / inOutRelativePath;
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

    RepositoryPtr FilesystemRepository::create(const lost::fs::Path& inRootDir)
    {
      return FilesystemRepositoryPtr(new FilesystemRepository(inRootDir));
    }

  }
}

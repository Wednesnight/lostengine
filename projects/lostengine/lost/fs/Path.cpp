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

#include "lost/fs/Path.h"
#include "lost/common/Logger.h"

namespace lost 
{
  namespace fs
  {

    static string dir_separator = "/";

    Path::Path() {}

    Path::Path(const lost::string& sp)
    : path(sp)
    {
    }

    Path::~Path() 
    {
    }

    void Path::operator /= (const Path& other)
    {
      path += dir_separator + other.path;
    }

    void Path::operator /= (const char* other)
    {
      path += dir_separator + other;
    }

    void Path::operator = (const char* other)
    {
      path = other;
    }

    bool Path::operator ==(const char* other)
    {
      return path == other;
    }

    bool Path::operator !=(const char* other)
    {
      return !this->operator==(other);
    }

    Path Path::branch_path()
    {
      Path result(path);
      string::size_type pos = path.rfind(dir_separator);
      if (pos != string::npos) {
        result = path.substr(0, pos);
      }
      return result;
    }

    lost::string Path::string() const
    {
      return path;
    }

    lost::string Path::native() const
    {
      return path;
    }

      
    Path operator / (const Path& left, const Path& right)
    {
      Path result(left.string());
      result /= right;
      return result;
    }

    bool exists(const Path& path)
    {
      // TODO
      return true;
    }
      
    bool create_directories(const Path& path)
    {
      // TODO
      return true;
    }

  }
}

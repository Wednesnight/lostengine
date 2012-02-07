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
#include "boost/filesystem.hpp"

namespace lost 
{
namespace fs
{

Path::Path() 
{
//  DOUT("");
//  _path = new boost::filesystem::path;
}

Path::Path(const lost::string& sp)
  : _path(sp)
{
//  DOUT("");
//  _path = new boost::filesystem::path(sp);  
}

Path::~Path() 
{
//  DOUT("");
//  delete _path;
}

void Path::operator /= (const Path& other)
{
  _path /= other._path;
}

void Path::operator /= (const char* other)
{
  _path /= other;
}

void Path::operator = (const char* other)
{
  _path = other;
}

bool Path::operator ==(const char* other)
{
  return _path == other;
}

bool Path::operator !=(const char* other)
{
  return !this->operator==(other);
}

Path Path::branch_path()
{
  Path result;
  result._path = _path.branch_path();
  return result;
}

lost::string Path::string() const
{
  return lost::string(_path.string().c_str());
}

lost::string Path::native() const
{
  // boost::filesystem::path::native() == boost::filesystem::path::m_pathname
  // boost::filesystem::path::c_str() == boost::filesystem::path::m_pathname.c_str()
  // boost::filesystem::path::string() == codecvt(m_pathname)
  return string();
}

  
Path operator / (const Path& left, const Path& right)
{
  Path result(left.string());
  result /= right;
  return result;
}

bool exists(const Path& path)
{
  return boost::filesystem::exists(path._path);
}
  
bool create_directories(const Path& path)
{
  return boost::filesystem::create_directories(path._path);
}

}
}

#ifndef LOST_FS_PATH_H
#define LOST_FS_PATH_H

#include "boost/filesystem.hpp"

namespace lost 
{
namespace fs
{

struct Path
{
  Path();
  Path(const lost::string& sp);
  virtual ~Path();

  void operator /= (const Path& other);
  lost::string string() const;
  lost::string native() const;
  
  boost::filesystem::path _path;
};

Path operator / (const Path& left, const Path& right);

bool exists(const Path& path);
bool create_directories(const Path& path); 
  
}
}

#endif
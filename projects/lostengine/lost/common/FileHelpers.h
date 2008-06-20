#ifndef LOST_COMMON_FILEHELPERS_H
#define LOST_COMMON_FILEHELPERS_H

#include <fstream>
#include <stdexcept>
#include <string>
#include "lost/platform/Platform.h"
#include "lost/common/ResourcePath.h"

namespace lost
{
namespace common
{
  // needs to have different name than the version below, because string and Resource Path are ambiguous because
  //RP is constructible from string.
  std::string loadFileAbsolute(const std::string& fullPath)
  {
    std::ifstream file(fullPath.c_str());
    if(!file.good())
    {
      throw std::runtime_error("couldn't open file: "+fullPath);
    }
    std::ostringstream os;
    os << file.rdbuf();

    return os.str();
  }
  
  // you can call this liek this: loadFile("realtive/path/to.file"); and it will automatically convert it 
  // to an absolute path inside your resource directory. Ain't that great?
  std::string loadFile(const lost::common::ResourcePath& inPath)
  {
    return loadFileAbsolute(inPath.str());
  }

}
}

#endif

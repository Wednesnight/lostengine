#include "lost/resource/Helper.h"
#include <fstream>
#include <stdexcept>
#include "lost/common/Logger.h"

namespace lost
{
namespace resource
{

common::DataPtr loadFromAbsolutePath(const std::string& inPath)
{
  common::DataPtr result(new common::Data);
  
  std::ifstream ifs( inPath.c_str(), std::ios_base::in | std::ios_base::binary );
  if(!ifs.good()) 
  {
    // common case: the file wasn't found in a particular repository and we just return NULL, no need to panic
//    DOUT("couldn't open file: '"+ inPath +"'");
    common::DataPtr null;
    return null;
  }
  ifs.seekg(0,std::ios_base::end);
  if(ifs.fail())
  {
    EOUT("seek failed!");
    common::DataPtr null;
    return null;
  }
  unsigned long filesize = ifs.tellg();
  ifs.seekg(std::ios_base::beg);
  result->size = filesize;
  result->bytes.reset(new char[filesize]);
  ifs.read(result->bytes.get(), filesize);
  
  return result;
}

}
}


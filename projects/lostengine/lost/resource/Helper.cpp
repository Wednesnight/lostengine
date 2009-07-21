#include "lost/resource/File.h"
#include "lost/resource/Helper.h"
#include <fstream>

namespace lost
{
namespace resource
{

FilePtr loadFromAbsolutePath(const std::string& inPath)
{
  FilePtr result(new File);
  
  std::ifstream ifs( inPath.c_str(), std::ios_base::in | std::ios_base::binary );
  if(!ifs.good()) { throw std::runtime_error( "couldn't open file: '"+ inPath +"'" ); }
  ifs.seekg(0,std::ios_base::end);
  if(ifs.fail()) throw std::runtime_error("failed");
  unsigned long filesize = ifs.tellg();
  ifs.seekg(std::ios_base::beg);
  result->size = filesize;
  result->data.reset(new char[filesize]);
  ifs.read(result->data.get(), filesize);
  
  return result;
}

}
}


#include "lost/resource/File.h"

namespace lost
{
namespace resource
{
File::File()
{
  size=0;
}

std::string File::str()
{ 
  return std::string(data.get(), size);
}
}
}

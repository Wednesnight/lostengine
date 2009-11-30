#include "lost/common/Data.h"

namespace lost
{
namespace common
{
Data::Data()
{
  size=0;
}

std::string Data::str()
{ 
  return std::string(bytes.get(), size);
}
}
}

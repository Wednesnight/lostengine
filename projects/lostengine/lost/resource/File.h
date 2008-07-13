#ifndef LOST_RESOURCE_FILE_H
#define LOST_RESOURCE_FILE_H

#include <string>
#include <boost/shared_array.hpp>
#include <stdint.h>

namespace lost
{
namespace resource
{
  struct File
  {
    boost::shared_array<char> data;
    uint32_t                  size;
    std::string               location;
    
    File() { size=0; }
    std::string str() { return std::string(data.get(), size); }
  };
}
}

#endif


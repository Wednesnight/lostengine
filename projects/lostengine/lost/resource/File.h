#ifndef LOST_RESOURCE_FILE_H
#define LOST_RESOURCE_FILE_H

#include <string>
#include <boost/shared_array.hpp>
#include <boost/cstdint.hpp>

namespace lost
{
namespace resource
{
  struct File
  {
    boost::shared_array<char> data;
	boost::uint32_t           size;
    std::string               location;
    
    File() { size=0; }
    std::string str() { return std::string(data.get(), size); }
  };
}
}

#endif


#ifndef LOST_RESOURCE_FILE_H
#define LOST_RESOURCE_FILE_H

#include <string>
#include <boost/cstdint.hpp>

namespace lost
{
namespace resource
{
  struct File;
  typedef boost::shared_ptr<File> FilePtr;

  struct File
  {
    boost::shared_array<char> data;
    boost::uint32_t           size;
    std::string               location;
    
    File();
    std::string str();
  };
}
}

#endif


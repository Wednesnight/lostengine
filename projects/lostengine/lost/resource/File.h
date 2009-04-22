#ifndef LOST_RESOURCE_FILE_H
#define LOST_RESOURCE_FILE_H

#include <string>
#include "lost/platform/shared_ptr.h"
#include <boost/shared_array.hpp>
#include <boost/cstdint.hpp>

namespace lost
{
namespace resource
{
  struct File;
  typedef lost::shared_ptr<File> FilePtr;

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


#ifndef LOST_COMMON_FILE_H
#define LOST_COMMON_FILE_H

#include <string>
#include "lost/platform/shared_ptr.h"
#include <boost/cstdint.hpp>

namespace lost
{
namespace common
{
  struct Data;
  typedef lost::shared_ptr<Data> DataPtr;

  struct Data
  {
    lost::shared_array<char> bytes;
    boost::uint32_t          size;
    
    Data();
    std::string str(); // returns data as string by stuffing the raw bytes into a string object
  };
}
}

#endif


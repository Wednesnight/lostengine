#ifndef LOST_COMMON_FILE_H
#define LOST_COMMON_FILE_H

#include <string>
#include "lost/platform/shared_ptr.h"
#include "lost/common/forward.h"

namespace lost
{
namespace common
{

  struct Data
  {
    lost::shared_array<char> bytes;
    uint32_t          size;
    
    Data();
    std::string str(); // returns data as string by stuffing the raw bytes into a string object
  };
}
}

#endif


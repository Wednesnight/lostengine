#ifndef LOST_COMMON_FILE_H
#define LOST_COMMON_FILE_H

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
      Data(const eastl::string& data);
      eastl::string str(); // returns data as string by stuffing the raw bytes into a string object

      static DataPtr create(const eastl::string& data) { return DataPtr(new Data(data)); }
    };
  }
}

#endif

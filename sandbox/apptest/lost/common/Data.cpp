#include "lost/common/Data.h"

namespace lost
{
  namespace common
  {

    Data::Data()
    {
      size = 0;
    }

    Data::Data(const eastl::string& data)
    {
      size = data.size();
      bytes.reset(new char[size]);
      data.copy(bytes.get(), size);
    }

    eastl::string Data::str()
    { 
      return eastl::string(bytes.get(), size);
    }

  }
}

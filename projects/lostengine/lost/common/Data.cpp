#include "lost/common/Data.h"

namespace lost
{
  namespace common
  {

    Data::Data()
    {
      size = 0;
    }

    Data::Data(const std::string& data)
    {
      size = data.size();
      bytes.reset(new char[size]);
      data.copy(bytes.get(), size);
    }

    std::string Data::str()
    { 
      return std::string(bytes.get(), size);
    }

  }
}

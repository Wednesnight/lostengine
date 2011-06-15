#include "lost/common/Data.h"

namespace lost
{
  namespace common
  {

    Data::Data()
    {
      size = 0;
    }

    Data::Data(const string& data)
    {
      size = data.size();
      bytes.reset(new char[size]);
      data.copy(bytes.get(), size);
    }

    string Data::str()
    { 
      return string(bytes.get(), size);
    }

  }
}

#include "lost/common/Hash.h"

namespace lost
{
namespace common
{

uint32_t djb2Hash(const char* data)
{
  uint32_t hash = 5381;
  int32_t c;

  const unsigned char* str = reinterpret_cast<const unsigned char*> (data);
  while ((c = *str++))
  {
      hash = ((hash << 5) + hash) + c;
  }

  return hash;  
}

}
}

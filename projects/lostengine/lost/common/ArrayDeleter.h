#ifndef LOST_COMMON_ARRAYDELETER_H
#define LOST_COMMON_ARRAYDELETER_H

namespace lost
{
namespace common
{
  /** use this to fake shared_array in lost namespace
   */
  template<typename T>
  struct ArrayDeleter
  {
    void operator()(T* p) const
    {
        if(p)
        {
            delete [] p;
        }
    }
  };
}
}

#endif
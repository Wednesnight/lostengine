#ifndef LOST_COMMON_NULLDELETER_H
#define LOST_COMMON_NULLDELETER_H

namespace lost
{
namespace common
{
  /** use this to create shared_ptrs that don't delete their guarded pointer on destruction.
   *  This is useful for non-pointer types or pointers that are created in a constructor from 'this'.
   */
  struct NullDeleter
  {
    void operator()(void const *) const
    {
    }
  };
}
}

#endif
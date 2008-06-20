#ifndef LOST_GUIRO_COMMON_CLONABLE_H
#define LOST_GUIRO_COMMON_CLONABLE_H

#include <boost/shared_ptr.hpp>

namespace lost
{
  namespace guiro
  {
    namespace common
    {

    struct Clonable
    {
      virtual ~Clonable() {}
      virtual Clonable* clone() = 0;
    };
    }
  } // namespace guiro
} // namespace lost

#endif

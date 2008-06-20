#ifndef LOST_GUIRO_IO_VEC2IO_H
#define LOST_GUIRO_IO_VEC2IO_H

#include "lost/math/Vec2.h"
#include <boost/serialization/nvp.hpp>

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::math::Vec2& vec, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(vec.x);
      ar &BOOST_SERIALIZATION_NVP(vec.y);
    }

  }
}

#endif

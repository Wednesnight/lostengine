#ifndef LOST_GUIRO_IO_COLORIO_H
#define LOST_GUIRO_IO_COLORIO_H

#include "lost/common/Color.h"
#include <boost/serialization/nvp.hpp>

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::common::Color& color, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(color.fv);
    }

  }
}

#endif


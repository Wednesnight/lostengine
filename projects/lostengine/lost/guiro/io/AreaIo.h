#ifndef LOST_GUIRO_IO_AREAIO_H
#define LOST_GUIRO_IO_AREAIO_H

#include "lost/guiro/common/Area.h"
#include <boost/serialization/nvp.hpp>
#include "lost/guiro/io/DimensionIo.h"

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::common::Area& area, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(area.x);
      ar &BOOST_SERIALIZATION_NVP(area.y);
      ar &BOOST_SERIALIZATION_NVP(area.width);
      ar &BOOST_SERIALIZATION_NVP(area.height);
    }

  }
}

#endif

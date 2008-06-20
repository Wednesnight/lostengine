#ifndef LOST_GUIRO_IO_QUADIO_H
#define LOST_GUIRO_IO_QUADIO_H

#include <boost/serialization/shared_ptr.hpp>
#include "lost/common/SerializationHelper.h"

#include "lost/guiro/control/Quad.h"
#include "lost/guiro/io/ViewIo.h"
#include "lost/io/Vec2Io.h"

BOOST_CLASS_EXPORT(lost::guiro::control::Quad)

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::control::Quad& quad, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(View, lost::guiro::control::View, quad);
      ar &BOOST_SERIALIZATION_NVP(quad.texture);
      ar &BOOST_SERIALIZATION_NVP(quad.bl);
      ar &BOOST_SERIALIZATION_NVP(quad.br);
      ar &BOOST_SERIALIZATION_NVP(quad.tl);
      ar &BOOST_SERIALIZATION_NVP(quad.tr);
    }

    VIEW_CONSTRUCT_DATA(lost::guiro::control::Quad)

  } // namespace serialization
} // namespace boost

#endif

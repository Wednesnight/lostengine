#ifndef LOST_GUIRO_IO_THEMEIO_H
#define LOST_GUIRO_IO_THEMEIO_H

#include <boost/serialization/map.hpp>
#include "lost/guiro/common/Theme.h"

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::common::Theme& theme, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(Object, lost::object::Object, theme);
      ar &BOOST_SERIALIZATION_NVP(theme.properties);
    }

  } // namespace serialization
} // namespace boost


#endif

#ifndef LOST_GUIRO_IO_IMAGEIO_H
#define LOST_GUIRO_IO_IMAGEIO_H

#include <boost/serialization/shared_ptr.hpp>
#include "lost/common/SerializationHelper.h"
#include "lost/guiro/control/Image.h"
#include "lost/guiro/io/ViewIo.h"

BOOST_CLASS_EXPORT(lost::guiro::control::Image)

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::control::Image& image, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(View, lost::guiro::control::View, image);
      ar &BOOST_SERIALIZATION_NVP(image.bitmap);
    }

    VIEW_CONSTRUCT_DATA(lost::guiro::control:: Image)

  } // namespace serialization
} // namespace boost


#endif

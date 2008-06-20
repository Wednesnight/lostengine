#ifndef LOST_GUIRO_IO_LABELIO_H
#define LOST_GUIRO_IO_LABELIO_H

#include <boost/serialization/shared_ptr.hpp>
#include "lost/common/SerializationHelper.h"

#include "lost/guiro/control/Label.h"
#include "lost/guiro/io/ViewIo.h"
#include "lost/io/ColorIo.h"

BOOST_CLASS_EXPORT(lost::guiro::control::Label)

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::control::Label& label, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(View, lost::guiro::control::View, label);
      ar &BOOST_SERIALIZATION_NVP(label.caption());
      ar &BOOST_SERIALIZATION_NVP(label.font());
      ar &BOOST_SERIALIZATION_NVP(label.fontColor());
      ar &BOOST_SERIALIZATION_NVP(label.fontSize());
    }

    VIEW_CONSTRUCT_DATA(lost::guiro::control::Label)

  } // namespace serialization
} // namespace boost


#endif

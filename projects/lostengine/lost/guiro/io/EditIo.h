#ifndef LOST_GUIO_IO_EDITIO_H
#define LOST_GUIO_IO_EDITIO_H

#include <boost/serialization/shared_ptr.hpp>
#include "lost/common/SerializationHelper.h"

#include "lost/guiro/control/Edit.h"
#include "lost/guiro/io/ViewIo.h"
#include "lost/io/ColorIo.h"

BOOST_CLASS_EXPORT(lost::guiro::control::Edit)

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::control::Edit& edit, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(View, lost::guiro::control::View, edit);
      ar &BOOST_SERIALIZATION_NVP(edit.background);
      ar &BOOST_SERIALIZATION_NVP(edit.font);
      ar &BOOST_SERIALIZATION_NVP(edit.fontColor);
      ar &BOOST_SERIALIZATION_NVP(edit.fontSize);
      ar &BOOST_SERIALIZATION_NVP(edit.caretVisible);
      ar &BOOST_SERIALIZATION_NVP(edit.caretDelay);
    }

    VIEW_CONSTRUCT_DATA(lost::guiro::control::Edit)

  } // namespace serialization
} // namespace boost


#endif

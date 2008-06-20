#ifndef LOST_GUIRO_IO_TEXTUREIO_H
#define LOST_GUIRO_IO_TEXTUREIO_H

#include "lost/guiro/common/Texture.h"

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::common::Texture& texture, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(Object, lost::object::Object, texture);
      ar &BOOST_SERIALIZATION_NVP(texture.bitmap);
    }

  } // namespace serialization
} // namespace boost


#endif

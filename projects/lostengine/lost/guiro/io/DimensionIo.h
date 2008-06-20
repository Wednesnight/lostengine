#ifndef LOST_GUIRO_IO_DIMENSIONIO_H
#define LOST_GUIRO_IO_DIMENSIONIO_H

#include "lost/guiro/common/Dimension.h"
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>
#include "lost/common/SerializationHelper.h"

BOOST_CLASS_EXPORT(lost::guiro::common::XDimension)
BOOST_CLASS_EXPORT(lost::guiro::common::leftedge)
BOOST_CLASS_EXPORT(lost::guiro::common::xcenter)
BOOST_CLASS_EXPORT(lost::guiro::common::xalignleft)
BOOST_CLASS_EXPORT(lost::guiro::common::xalignright)
BOOST_CLASS_EXPORT(lost::guiro::common::xabs)
BOOST_CLASS_EXPORT(lost::guiro::common::xrel)
BOOST_CLASS_EXPORT(lost::guiro::common::YDimension)
BOOST_CLASS_EXPORT(lost::guiro::common::bottomedge)
BOOST_CLASS_EXPORT(lost::guiro::common::ycenter)
BOOST_CLASS_EXPORT(lost::guiro::common::yalignbottom)
BOOST_CLASS_EXPORT(lost::guiro::common::yaligntop)
BOOST_CLASS_EXPORT(lost::guiro::common::yabs)
BOOST_CLASS_EXPORT(lost::guiro::common::yrel)
BOOST_CLASS_EXPORT(lost::guiro::common::WDimension)
BOOST_CLASS_EXPORT(lost::guiro::common::width)
BOOST_CLASS_EXPORT(lost::guiro::common::wabs)
BOOST_CLASS_EXPORT(lost::guiro::common::wrel)
BOOST_CLASS_EXPORT(lost::guiro::common::HDimension)
BOOST_CLASS_EXPORT(lost::guiro::common::height)
BOOST_CLASS_EXPORT(lost::guiro::common::habs)
BOOST_CLASS_EXPORT(lost::guiro::common::hrel)


#define SERIALIZE_DIMENSION(classname, baseclass)                                                        \
template<class Archive>                                                                                \
inline void serialize( Archive& ar, lost::guiro::common::classname& dimension, const unsigned int version )    \
{                                                                                                      \
ar &BOOST_SERIALIZATION_NON_INTRUSIVE_BASE_OBJECT_NVP(baseclass, lost::guiro::common::baseclass, dimension); \
}                                                                                                      \

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::common::Dimension& dimension, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(dimension.relative);
      ar &BOOST_SERIALIZATION_NVP(dimension.absolute);
    }

    SERIALIZE_DIMENSION(XDimension, Dimension)
    SERIALIZE_DIMENSION(leftedge, XDimension)
    SERIALIZE_DIMENSION(xcenter, XDimension)
    SERIALIZE_DIMENSION(xalignleft, XDimension)
    SERIALIZE_DIMENSION(xalignright, XDimension)
    SERIALIZE_DIMENSION(xabs, leftedge)
    SERIALIZE_DIMENSION(xrel, leftedge)
    SERIALIZE_DIMENSION(YDimension, Dimension)
    SERIALIZE_DIMENSION(bottomedge, YDimension)
    SERIALIZE_DIMENSION(ycenter, YDimension)
    SERIALIZE_DIMENSION(yalignbottom, YDimension)
    SERIALIZE_DIMENSION(yaligntop, YDimension)
    SERIALIZE_DIMENSION(yabs, bottomedge)
    SERIALIZE_DIMENSION(yrel, bottomedge)
    SERIALIZE_DIMENSION(WDimension, Dimension)
    SERIALIZE_DIMENSION(width, WDimension)
    SERIALIZE_DIMENSION(wabs, width)
    SERIALIZE_DIMENSION(wrel, width)
    SERIALIZE_DIMENSION(HDimension, Dimension)
    SERIALIZE_DIMENSION(height, HDimension)
    SERIALIZE_DIMENSION(habs, height)
    SERIALIZE_DIMENSION(hrel, height)

  }
}


#endif

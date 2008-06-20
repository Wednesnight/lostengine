#ifndef LOST_GUIRO_IO_VIEWIO_H
#define LOST_GUIRO_IO_VIEWIO_H

#include <boost/serialization/list.hpp>
#include "lost/guiro/control/View.h"
#include "lost/guiro/io/AreaIo.h"
#include "lost/io/Vec2Io.h"
#include "lost/io/ColorIo.h"

#define VIEW_CONSTRUCT_DATA(classname) \
template<class Archive>                                                                               \
inline void save_construct_data(Archive & ar, const classname* view, const unsigned int file_version) \
{                                                                                                     \
std::string name(view->name);                                                                       \
ar << BOOST_SERIALIZATION_NVP(name);                                                                \
}                                                                                                     \
template<class Archive>                                                                               \
inline void load_construct_data(Archive & ar, classname* view, const unsigned int file_version)       \
{                                                                                                     \
std::string name;                                                                                   \
ar >> BOOST_SERIALIZATION_NVP(name);                                                                \
::new(view)classname(name);                                                                         \
}                                                                                                     \

namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::guiro::control::View& view, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(view.area);
      ar &BOOST_SERIALIZATION_NVP(view.unsortedChildren);
      ar &BOOST_SERIALIZATION_NVP(view.color);
      ar &BOOST_SERIALIZATION_NVP(view.visible);
      ar &BOOST_SERIALIZATION_NVP(view.movable);
      ar &BOOST_SERIALIZATION_NVP(view.sizable);
      ar &BOOST_SERIALIZATION_NVP(view.draggable);
    }

    VIEW_CONSTRUCT_DATA(lost::guiro::control::View)

  } // namespace serialization
} // namespace boost


#endif

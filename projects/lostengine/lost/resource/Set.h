#ifndef LOST_RESOURCE_SET_H
#define LOST_RESOURCE_SET_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>

namespace lost
{
  namespace resource
  {

    template <typename Object>
    struct Set
    {
      std::vector<boost::shared_ptr<Object> > objects;
    };

  } // namespace resource
} // namespace lost


namespace boost
{
  namespace serialization
  {

    template<class Archive, typename Object>
    inline void serialize( Archive& ar, lost::resource::Set<Object>& set, const unsigned int version )
    {
      std::vector<boost::shared_ptr<Object> >& objects = set.objects;
      ar &BOOST_SERIALIZATION_NVP(objects);
    }

  } // namespace serialization
} // namespace boost

#endif

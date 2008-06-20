#ifndef LOST_OBJECT_OBJECT_H
#define LOST_OBJECT_OBJECT_H

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "lost/common/Logger.h"

namespace lost
{
  namespace object
  {

    struct Object
    {
      std::string id;
      std::string location;

      Object()
      {
        resolved = false;
      }

      virtual ~Object()
      {
        DOUT("Object Destructor (" << id <<  " / " << location << ")");
      }

      // this is called once the resolve of the current object and all it's children is finished.
      // Override this to perform further initialisation required by the derived class.
      virtual void resolveFinished()
      {
      }

      bool getResolved() { return resolved; }
      void setResolved(bool inVal) { resolved = inVal; }

    private:
      bool        resolved;
    };

  } // namespace object
} // namespace lost


namespace boost
{
  namespace serialization
  {

    template<class Archive>
    inline void serialize( Archive& ar, lost::object::Object& object, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(object.id);
      ar &BOOST_SERIALIZATION_NVP(object.location);
    }

  } // namespace serialization
} // namespace boost

#endif

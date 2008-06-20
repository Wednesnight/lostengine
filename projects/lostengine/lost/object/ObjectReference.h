#ifndef LOST_OBJECT_OBJECTREFERENCE_H
#define LOST_OBJECT_OBJECTREFERENCE_H

#include <boost/shared_ptr.hpp>

#include "lost/object/ObjectReferenceResolver.h"
#include "lost/object/ObjectReferenceContainer.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace object
  {

    template<typename ObjectType>
    struct ObjectReference
    {
      std::string                   type;
      std::string                   id;
      boost::shared_ptr<ObjectType> pointer;

      ObjectReference()
      {
      }

      virtual ~ObjectReference() {}

      void resolve( ObjectReferenceResolver& resolver )
      {
        if(type.empty()) { WOUT("tried to resolve reference (id: "+ id +") with empty type!"); return; }

        DOUT(type <<" " <<  id << " : " << this << " " << pointer << " " << (pointer ? pointer->getResolved() : false) );
        if(!pointer or !pointer->getResolved())
        {
          DOUT( "resolving: " << type << ", " << id);
          pointer = resolver.resolve<ObjectType>( type, id );
          lost::object::ObjectReferenceContainer* base = dynamic_cast<ObjectReferenceContainer*>(pointer.get());
          if(base)
          {
            DOUT("resolving recursively from " << type << " " << id);
            base->resolve(resolver);
          }
          if(!pointer->getResolved()) // check resolved flag again here so we only call resolvedFinished() once
          {
            pointer->setResolved(true);
            pointer->resolveFinished();
          }
          else
          {
            DOUT("prevented possibly unwanted call to resolveFinished for " << type << " " << id);
          }
        }
        else
        {
          DOUT("prevented multiple resolve of " << type << " " << id);
        }
      }

      /** IMPORTANT! use this to check if the reference is valid before accessing any members.
       * e.g.
       * ObjectReference<Theme> theme;
       * if(theme)
       *   this->add(theme->view);
       *
       */
      operator void const* () const
      {
        if(pointer)
          return this;
        else
          return 0;
      }

      ObjectType* operator->()
      {
        return pointer.get();
      }

      ObjectType& operator*()
      {
        return *pointer;
      }
    };

  }
}


namespace boost
{
  namespace serialization
  {

    template<class Archive, typename Object>
    inline void serialize( Archive& ar, lost::object::ObjectReference<Object>& reference, const unsigned int version )
    {
      ar &BOOST_SERIALIZATION_NVP(reference.type);
      ar &BOOST_SERIALIZATION_NVP(reference.id);
    }

  } // namespace serialization
} // namespace boost

#endif

#ifndef LOST_OBJECT_OBJECTREFERENCERESOLVER_H
#define LOST_OBJECT_OBJECTREFERENCERESOLVER_H

#include <boost/shared_ptr.hpp>

#include "lost/guiro/factory/Factory.h"
#include <stdexcept>

namespace lost
{
  namespace object
  {

    struct ObjectReferenceResolver
    {
      void registerFactory( const std::string& inId, boost::shared_ptr<guiro::factory::Factory> inFactory )
      {
        factories[inId] = inFactory;
      }

      // it's actually kinda odd that we have a ObjectReference type that encapsulates "type" and "id"
      // along with other stuff, but use the two strings here directly. At first, this seemed like an oversight
      // to me, but on the other hand it's pretty cool: it breaks the circular dependency to ObjectReference!
      // Maybe we should keep it this way ... ?
      template <typename ObjectType>
      boost::shared_ptr<ObjectType> resolve( const std::string& inType, const std::string& inId )
      {
        std::map<std::string, boost::shared_ptr<guiro::factory::Factory> >::iterator pos = factories.find(inType);
        if(pos != factories.end())
        {
          return boost::dynamic_pointer_cast<ObjectType>( (*(pos->second))[inId] );
        }
        else
        {
          throw std::runtime_error("resolve: couldn't find factory for type: "+ inType +", id: "+ inId);
        }

      }

      std::map<std::string, boost::shared_ptr<guiro::factory::Factory> > factories;

    };

  }
}

#endif


#ifndef LOST_OBJECT_OBJECTREFERENCECONTAINER_H
#define LOST_OBJECT_OBJECTREFERENCECONTAINER_H

#include <boost/shared_ptr.hpp>

#include "lost/object/ObjectReferenceResolver.h"

namespace lost
{
  namespace object
  {

    struct ObjectReferenceContainer
    {
      virtual ~ObjectReferenceContainer() {}
      virtual void resolve( ObjectReferenceResolver& inResolver ) = 0;
    };

  }
}

#endif


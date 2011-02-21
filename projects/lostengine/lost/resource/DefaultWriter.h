#ifndef LOST_RESOURCE_DEFAULTWRITER_H
#define LOST_RESOURCE_DEFAULTWRITER_H

#include "lost/resource/Writer.h"

namespace lost
{
  namespace resource
  {
    struct DefaultWriter : public Writer
    {
      DefaultWriter();
      virtual ~DefaultWriter();
      
      static DefaultWriterPtr create() { return DefaultWriterPtr(new DefaultWriter()); }
    };
  }
}

#endif

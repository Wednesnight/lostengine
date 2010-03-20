#ifndef LOST_APPLICATION_QUEUEENTITY_H
#define LOST_APPLICATION_QUEUEENTITY_H

#include "lost/application/forward.h"

namespace lost
{
  namespace application
  {
    struct QueueEntity
    {
      virtual void process(const Tasklet* sender) const = 0;
    };
  }
}

#endif

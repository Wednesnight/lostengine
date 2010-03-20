#ifndef LOST_APPLICATION_QUEUE_H
#define LOST_APPLICATION_QUEUE_H

#include <vector>
#include "lost/application/forward.h"
#include "lost/application/QueueEntity.h"

namespace lost
{
  namespace application
  {
    struct Queue : public std::vector<const QueueEntity*>
    {
      ~Queue();
      void queue(const QueueEntity* entity);
      void process(const Tasklet* sender);
      void clear();
    };
  }
}

#endif

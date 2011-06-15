#ifndef LOST_APPLICATION_QUEUE_H
#define LOST_APPLICATION_QUEUE_H

#include "lost/application/forward.h"
#include "lost/application/QueueEntity.h"

namespace lost
{
  namespace application
  {
    struct Queue : public vector<const QueueEntity*>
    {
      ~Queue();
      void queue(const QueueEntity* entity);
      void process(const Tasklet* sender);
      void clear();
    };
  }
}

#endif

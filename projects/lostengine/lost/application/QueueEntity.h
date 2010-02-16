#ifndef LOST_APPLICATION_QUEUEENTITY_H
#define LOST_APPLICATION_QUEUEENTITY_H

namespace lost
{
  namespace application
  {
    struct Tasklet;
    struct QueueEntity
    {
      virtual void process(const Tasklet* sender) const = 0;
    };
  }
}

#endif

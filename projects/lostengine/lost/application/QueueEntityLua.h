#ifndef LOST_APPLICATION_QUEUEENTITYLUA_H
#define LOST_APPLICATION_QUEUEENTITYLUA_H

#include "lost/application/forward.h"
#include "lost/application/QueueEntity.h"
#include "lost/lua/lua.h"

namespace lost
{
  namespace application
  {
    struct QueueEntityLua : public QueueEntity
    {

    private:
      slub::reference targetObject;
      slub::reference targetMethod;

    public:
      QueueEntityLua(const slub::reference& targetObject, const slub::reference& targetMethod);
      void process(const Tasklet* sender) const;

    };
  }
}

#endif

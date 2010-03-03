#ifndef LOST_APPLICATION_QUEUEENTITYLUA_H
#define LOST_APPLICATION_QUEUEENTITYLUA_H

#include "lost/application/QueueEntity.h"

namespace luabind { class object; }

namespace lost
{
  namespace application
  {
    struct Tasklet;
  
    struct QueueEntityLua : public QueueEntity
    {

    private:
      luabind::object targetObject;
      luabind::object targetMethod;

    public:
      QueueEntityLua(const luabind::object& targetObject, const luabind::object& targetMethod);
      void process(const Tasklet* sender) const;

    };
  }
}

#endif

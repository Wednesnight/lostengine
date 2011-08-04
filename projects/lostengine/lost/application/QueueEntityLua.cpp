#include "lost/lua/lua.h"
#include "lost/application/Tasklet.h"
#include "lost/application/QueueEntityLua.h"

using namespace slub;

namespace lost
{
  namespace application
  {

    QueueEntityLua::QueueEntityLua(const reference& targetObject, const reference& targetMethod)
    {
      this->targetObject = targetObject;
      this->targetMethod = targetMethod;
    }

    void QueueEntityLua::process(const Tasklet* sender) const
    {
      if (targetMethod.type() == LUA_TFUNCTION)
      {
        if (targetObject.type() != LUA_TNIL)
        {
          targetMethod(targetObject, sender);
        }
        else
        {
          targetMethod(sender);
        }

      }
    }

  }
}

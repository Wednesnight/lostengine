#include "lost/lua/lua.h"
#include "lost/application/Tasklet.h"
#include "lost/application/QueueEntityLua.h"

using namespace luabind;

namespace lost
{
  namespace application
  {

    QueueEntityLua::QueueEntityLua(const object& targetObject, const object& targetMethod)
    {
      this->targetObject = targetObject;
      this->targetMethod = targetMethod;
    }

    void QueueEntityLua::process(const Tasklet* sender) const
    {
      if (type(targetMethod) == LUA_TFUNCTION)
      {
        if (type(targetObject) != LUA_TNIL)
        {
          call_function<void>(targetMethod, targetObject, sender);
        }
        else
        {
          call_function<void>(targetMethod, sender);
        }

      }
    }

  }
}

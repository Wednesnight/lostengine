#include "lost/event/EventDispatcher.h"
#include "lost/lua/lua.h"
#include "lost/application/Tasklet.h"

#include "lost/lua/bindings/LostApplicationTasklet.h"

using namespace luabind;
using namespace lost::application;

namespace lost
{
  namespace lua
  {
    void LostApplicationTasklet(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("application")
        [
          class_<Tasklet>("Tasklet")
            .def_readonly("eventDispatcher", &Tasklet::eventDispatcher)
        ]
      ];
    }
  }
}
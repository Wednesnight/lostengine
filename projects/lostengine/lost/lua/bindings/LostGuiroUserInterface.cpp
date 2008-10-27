#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/guiro/UserInterface.h"

#include "lost/lua/bindings/LostGuiroUserInterface.h"

using namespace luabind;
using namespace lost::guiro;

namespace lost
{
  namespace lua
  {
    void LostGuiroUserInterface(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("guiro")
        [
          class_<UserInterface, View, boost::shared_ptr<UserInterface> >("UserInterface")
            .def(constructor<>())
        ]
      ];
    }
  }
}

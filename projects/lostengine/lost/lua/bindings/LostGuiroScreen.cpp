#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/guiro/Screen.h"

#include "lost/lua/bindings/LostGuiroScreen.h"

using namespace luabind;
using namespace lost::guiro;

namespace lost
{
  namespace lua
  {
    void LostGuiroScreen(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("guiro")
        [
          class_<Screen, View, boost::shared_ptr<Screen>, boost::shared_ptr<View> >("Screen")
            .def(constructor<const boost::shared_ptr<lost::lua::State>&, const boost::shared_ptr<lost::common::DisplayAttributes>&>())
        ]
      ];
    }
  }
}

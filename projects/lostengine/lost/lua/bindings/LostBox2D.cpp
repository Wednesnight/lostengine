#include "lost/lua/lua.h"
#include "lost/lua/bindings/LostBox2D.h"
#include "lost/box2d/DebugDraw.h"
#include "lost/rg/Node.h"

using namespace luabind;
using namespace lost::box2d;

namespace lost
{
namespace lua
{

void LostBox2D(lua_State* state)
{
  module(state, "lost")
  [
    namespace_("box2d")
    [
      class_<DebugDraw,b2DebugDraw>("DebugDraw")
      .def(constructor<resource::LoaderPtr>())
      .def("beginDraw",&DebugDraw::beginDraw)
      .def("endDraw",&DebugDraw::endDraw)
      .def_readonly("rootNode",&DebugDraw::rootNode)
    ]
  ];
}

}
}

#include "lost/lua/lua.h"
#include "lost/mesh/Line.h"

#include "lost/lua/bindings/LostMeshLine.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
    void LostMeshLine(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Line2D, Line2DPtr >("Line2D")
           .def_readwrite("material", &Line2D::material)
          .scope
          [
           def("create", (Line2DPtr(*)()) &Line2D::create),
           def("create", (Line2DPtr(*)(const math::Vec2&, const math::Vec2&)) &Line2D::create)
          ]
        ]
      ];
    }
  }
}

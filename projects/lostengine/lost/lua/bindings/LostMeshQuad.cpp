#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/mesh/Quad.h"

#include "lost/lua/bindings/LostMeshQuad.h"

using namespace luabind;
using namespace lost::mesh;

namespace lost
{
  namespace lua
  {
    void LostMeshQuad(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("mesh")
        [
          class_<Quad2D, Quad2DPtr >("Quad2D")
          .scope
          [
           def("create", (Quad2DPtr(*)()) &Quad2D::create),
           def("create", (Quad2DPtr(*)(const math::Rect& inRect)) &Quad2D::create),
           def("create", (Quad2DPtr(*)(resource::FilePtr data, bool)) &Quad2D::create),
           def("create", (Quad2DPtr(*)(gl::TexturePtr tex, bool)) &Quad2D::create)
          ]
        ]
      ];
    }
  }
}

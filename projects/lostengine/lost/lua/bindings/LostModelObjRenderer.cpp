#include "lost/lua/lua.h"
#include "lost/model/obj/Renderer.h"

#include "lost/lua/bindings/LostModelObjRenderer.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::obj;

namespace lost
{
  namespace lua
  {
    void LostModelObjRenderer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          namespace_("obj")
          [
            class_<Renderer, lost::shared_ptr<Renderer> >("Renderer")
            .def(constructor<lost::shared_ptr<lost::gl::Canvas>, lost::shared_ptr<Mesh>, lost::shared_ptr<Material> >()) 
            .def_readwrite("size", &Renderer::size)
          ]
        ]
      ];
    }
  }
}

#include "lost/lua/bindings/LostModelObjRenderer.h"
#include "lost/model/obj/Renderer.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::obj;

LOST_LUA_EXPORT_BEGIN(LostModelObjRenderer)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      namespace_("obj")
      [
        class_<Renderer, boost::shared_ptr<Renderer> >("Renderer")
        .def(constructor<boost::shared_ptr<Mesh>, boost::shared_ptr<Material> >()) 
        .def_readwrite("size", &Renderer::size)
      ]
    ]
  ];
}
LOST_LUA_EXPORT_END

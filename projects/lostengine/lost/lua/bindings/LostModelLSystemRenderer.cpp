#include "lost/lua/bindings/LostModelLSystemRenderer.h"
#include "lost/model/lsystem/Renderer.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::lsystem;

LOST_LUA_EXPORT_BEGIN(LostModelLSystemRenderer)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      namespace_("lsystem")
      [
        class_<Renderer, boost::shared_ptr<Renderer> >("Renderer")
        .def(constructor<const boost::shared_ptr<lost::gl::Context>&, const boost::shared_ptr<Mesh>&>()) 
        .def("render", &Renderer::render)
        .def_readwrite("size", &Renderer::size)
      ]
    ]
  ];
}
LOST_LUA_EXPORT_END

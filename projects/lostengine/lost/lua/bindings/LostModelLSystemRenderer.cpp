#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/model/lsystem/Renderer.h"

#include "lost/lua/bindings/LostModelLSystemRenderer.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::lsystem;

namespace lost
{
  namespace lua
  {
    void LostModelLSystemRenderer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          namespace_("lsystem")
          [
            class_<Renderer, boost::shared_ptr<Renderer> >("Renderer")
            .def(constructor<const boost::shared_ptr<lost::gl::Canvas>&, const boost::shared_ptr<Mesh>&>()) 
            .def("render", &Renderer::render)
            .def_readwrite("size", &Renderer::size)
          ]
        ]
      ];
    }
  }
}

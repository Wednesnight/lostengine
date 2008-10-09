#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/model/obj/Renderer.h"

#include "lost/lua/bindings/LostModelObjRenderer.h"

using namespace luabind;
using namespace lost::model;
using namespace lost::model::obj;

namespace lost
{
  namespace lua
  {
    void LostModelObjRenderer(lost::lua::State& state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          namespace_("obj")
          [
            class_<Renderer, boost::shared_ptr<Renderer> >("Renderer")
            .def(constructor<boost::shared_ptr<lost::gl::Context>, boost::shared_ptr<Mesh>, boost::shared_ptr<Material> >()) 
            .def_readwrite("size", &Renderer::size)
          ]
        ]
      ];
    }
  }
}

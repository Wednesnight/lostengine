#include <boost/shared_ptr.hpp>
#include "lost/lua/State.h"
#include "lost/model/Mesh.h"

#include "lost/lua/bindings/LostModelMesh.h"

using namespace luabind;
using namespace lost::model;

namespace lost
{
  namespace lua
  {
    void LostModelMesh(lost::lua::State& state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          class_<Mesh, boost::shared_ptr<Mesh> >("Mesh")
          .def(constructor<>()) 
          .def(constructor<unsigned int, unsigned int>()) 
          //.def("get", &TouchEvent::get) 
          //.def_readwrite("touches", &TouchEvent::touches)
        ]
      ];
    }
  }
}

#include "lost/lua/lua.h"
#include "lost/model/Mesh.h"

#include "lost/lua/bindings/LostModelMesh.h"

using namespace luabind;
using namespace lost::model;

namespace lost
{
  namespace lua
  {
    void LostModelMesh(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("model")
        [
          class_<Mesh, lost::shared_ptr<Mesh> >("Mesh")
          .def(constructor<>()) 
          .def(constructor<unsigned int, unsigned int>()) 
          //.def("get", &TouchEvent::get) 
          //.def_readwrite("touches", &TouchEvent::touches)
        ]
      ];
    }
  }
}

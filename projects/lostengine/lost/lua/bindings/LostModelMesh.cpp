#include "lost/lua/bindings/LostModelMesh.h"
#include "lost/model/Mesh.h"

using namespace luabind;
using namespace lost::model;

LOST_LUA_EXPORT_BEGIN(LostModelMesh)
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
LOST_LUA_EXPORT_END

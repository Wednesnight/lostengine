#include "lost/lua/LuaBindings.h"
#include "lost/model/RendererOBJ.h"

using namespace luabind;
using namespace lost::model;

LOST_LUA_EXPORT_BEGIN(LuaLostModelRendererOBJ)
{
  module(state, "lost")
  [
    namespace_("model")
    [
      class_<RendererOBJ, boost::shared_ptr<RendererOBJ> >("RendererOBJ")
      .def(constructor<boost::shared_ptr<Mesh>, boost::shared_ptr<MaterialOBJ> >()) 
      .def_readwrite("size", &RendererOBJ::size)
    ]
  ];
}
LOST_LUA_EXPORT_END

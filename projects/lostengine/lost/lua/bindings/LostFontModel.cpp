#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/font/Model.h"

#include "lost/lua/bindings/LostFontModel.h"

using namespace luabind;
using namespace lost::font;

namespace lost
{
  namespace lua
  {
    void LostFontModel(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<Model, boost::shared_ptr<Model> >("Model")
            .def(constructor<>())
            .def("render", &Model::render)
            .def_readonly("min", &Model::min)
            .def_readonly("max", &Model::max)
            .def_readonly("size", &Model::size)
        ]
      ];
    }
  }
}

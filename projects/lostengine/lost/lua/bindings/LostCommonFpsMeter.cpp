#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/common/FpsMeter.h"

#include "lost/lua/bindings/LostCommonFpsMeter.h"

using namespace luabind;
using namespace lost::common;

namespace lost
{
  namespace lua
  {
    void LostCommonFpsMeter(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("common")
        [
          class_<FpsMeter, boost::shared_ptr<FpsMeter> >("FpsMeter")
          .def(constructor<boost::shared_ptr<lost::gl::Context> >()) 
          .def("render", &FpsMeter::render)
          .def_readwrite("width", &FpsMeter::width)
          .def_readwrite("height", &FpsMeter::height)
          .def_readwrite("alpha", &FpsMeter::alpha)      
        ]
      ];
    }
  }
}

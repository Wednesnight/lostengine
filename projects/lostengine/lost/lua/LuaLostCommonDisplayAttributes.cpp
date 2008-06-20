#include "lost/lua/Luabindings.h"
#include "lost/common/DisplayAttributes.h"

using namespace std;
using namespace luabind;
using namespace lost::common;

namespace lost
{
namespace lua
{

void bindLostCommonDisplayAttributes(lost::lua::State& state)
{
  module(state, "lost")
  [
    namespace_("common")
    [
      class_<DisplayAttributes>("DisplayAttributes")
        .def(constructor<>())    
         .def_readwrite("width",     &DisplayAttributes::width)
         .def_readwrite("height",     &DisplayAttributes::height)
         .def_readwrite("redbits",     &DisplayAttributes::redbits)
         .def_readwrite("bluebits",     &DisplayAttributes::bluebits)
         .def_readwrite("greenbits",     &DisplayAttributes::greenbits)
         .def_readwrite("alphabits",     &DisplayAttributes::alphabits)
         .def_readwrite("depthbits",     &DisplayAttributes::depthbits)
         .def_readwrite("stencilbits",  &DisplayAttributes::stencilbits)
         .def_readwrite("fullscreen",  &DisplayAttributes::fullscreen)
         .def_readwrite("title",  &DisplayAttributes::title)
    ]
  ];
}

}
}

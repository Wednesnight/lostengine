#include "lost/lua/bindings/LostCommonDisplayAttributes.h"
#include "lost/common/DisplayAttributes.h"

using namespace luabind;
using namespace lost::common;

LOST_LUA_EXPORT_BEGIN(LostCommonDisplayAttributes)
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
LOST_LUA_EXPORT_END

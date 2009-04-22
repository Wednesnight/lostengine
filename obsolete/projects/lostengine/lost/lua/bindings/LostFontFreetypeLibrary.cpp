#include "lost/lua/lua.h"
#include "lost/lua/bindings/LostFontFreetypeLibrary.h"
#include "lost/font/freetype/Library.h"

using namespace luabind;
using namespace lost;
using namespace lost::font::freetype;

shared_ptr<Library> create()
{
  return shared_ptr<Library>(new Library);
}

namespace lost
{
  namespace lua
  {
    void LostFontFreetypeLibrary(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          namespace_("freetype")
          [
            class_<Library, lost::shared_ptr<Library> >("Library")
              .def(constructor<>())
              .scope
              [
                def("create", create)
              ]
          ]
        ]
      ];
    }
  }
}

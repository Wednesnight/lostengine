#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/lua/bindings/LostFontFreetypeLibrary.h"
#include "lost/font/freetype/Library.h"

using namespace luabind;
using namespace boost;
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
            class_<Library, boost::shared_ptr<Library> >("Library")
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

#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/font/Model.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/lua/bindings/LostFontTrueTypeFont.h"

using namespace luabind;
using namespace boost;
using namespace lost::font;

shared_ptr<TrueTypeFont> create(shared_ptr<freetype::Library> lib, shared_ptr<lost::resource::File> file)
{
  return shared_ptr<TrueTypeFont>(new TrueTypeFont(lib, file));
}

namespace lost
{
  namespace lua
  {
    void LostFontTrueTypeFont(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<TrueTypeFont, shared_ptr<TrueTypeFont> >("TrueTypeFont")
            .def(constructor<shared_ptr<freetype::Library>, shared_ptr<resource::File> >())
            .def("render", &TrueTypeFont::render)
            .scope
            [
              def("create", create)
            ]
        ]
      ];
    }
  }
}

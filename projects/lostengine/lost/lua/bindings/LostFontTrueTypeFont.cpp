#include <boost/shared_ptr.hpp>
#include "lost/lua/lua.h"
#include "lost/font/Model.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/lua/bindings/LostFontTrueTypeFont.h"

using namespace luabind;
using namespace boost;
using namespace lost::font;

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
          class_<TrueTypeFont, boost::shared_ptr<TrueTypeFont> >("TrueTypeFont")
            .def(constructor<shared_ptr<freetype::Library>, shared_ptr<resource::File> >())
            .def("render", &TrueTypeFont::render)
        ]
      ];
    }
  }
}

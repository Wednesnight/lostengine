#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/lua.h"

#include "lost/font/freetype/Library.h"
#include "lost/font/Model.h"
#include "lost/font/TrueTypeFont.h"

using namespace luabind;
using namespace lost;
using namespace lost::font;
using namespace lost::font::freetype;

namespace lost
{
  namespace lua
  {

    shared_ptr<Library> createLib()
    {
      return shared_ptr<Library>(new Library);
    }
    
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
                def("create", createLib)
              ]
          ]
        ]
      ];
    }

    void LostFontModel(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<Model, lost::shared_ptr<Model> >("Model")
            .def(constructor<>())
            .def("render", &Model::render)
            .def_readonly("min", &Model::min)
            .def_readonly("max", &Model::max)
            .def_readonly("size", &Model::size)
        ]
      ];
    }

    shared_ptr<TrueTypeFont> createTTF(shared_ptr<freetype::Library> lib, shared_ptr<lost::resource::File> file)
    {
      return shared_ptr<TrueTypeFont>(new TrueTypeFont(lib, file));
    }

    typedef lost::shared_ptr<Model> (TrueTypeFont::*render_func_t)(const std::string &, boost::uint32_t);

    void LostFontTrueTypeFont(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<TrueTypeFont, shared_ptr<TrueTypeFont> >("TrueTypeFont")
            .def(constructor<shared_ptr<freetype::Library>, shared_ptr<resource::File> >())
            .def("render", static_cast<render_func_t>(&TrueTypeFont::render))
            .scope
            [
              def("create", createTTF)
            ]
        ]
      ];
    }
    
    void LostFont(lua_State* state)
    {
      LostFontFreetypeLibrary(state);
      LostFontModel(state);
      LostFontTrueTypeFont(state);
    }

  }
}

#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/lua.h"
#include "lost/mesh/Mesh.h"
#include "lost/font/freetype/Library.h"
#include "lost/font/TrueTypeFont.h"

using namespace luabind;
using namespace lost;
using namespace lost::font;
using namespace lost::mesh;
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
            class_<Library>("Library")
              .def(constructor<>())
              .scope
              [
                def("create", createLib)
              ]
          ]
        ]
      ];
    }

    void LostFontRenderedText(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<RenderedText, Mesh>("RenderedText")
            .def_readonly("min", &RenderedText::min)
            .def_readonly("max", &RenderedText::max)
            .def_readonly("size", &RenderedText::size)
            .scope
            [
              def("create", &RenderedText::create)
            ]
        ]
      ];
    }

    shared_ptr<TrueTypeFont> createTTF(shared_ptr<freetype::Library> lib, shared_ptr<lost::resource::File> file)
    {
      return shared_ptr<TrueTypeFont>(new TrueTypeFont(lib, file));
    }

    RenderedTextPtr renderCrippled(TrueTypeFontPtr ttf, const std::string & str, boost::uint32_t sz)
    {
      return ttf->render(str, sz);
    }

    void renderToTarget(TrueTypeFontPtr ttf, 
                        const std::string & inText,
                        boost::uint32_t inSizeInPoints,
                        RenderedTextPtr target)
    {
      ttf->render(inText, inSizeInPoints, target);
    }


    typedef RenderedTextPtr (TrueTypeFont::*render_func_t)(const std::string &, boost::uint32_t);

    void LostFontTrueTypeFont(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<TrueTypeFont>("TrueTypeFont")
            .def(constructor<shared_ptr<freetype::Library>, shared_ptr<resource::File> >())
            .def("render", renderCrippled)
            .def("render", renderToTarget)
            .def_readwrite("atlasSize", &TrueTypeFont::atlasSize)
            .def_readonly("atlas", &TrueTypeFont::atlas)
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
      LostFontRenderedText(state);
      LostFontTrueTypeFont(state);
    }

  }
}

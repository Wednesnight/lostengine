#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/lua.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/font/RenderedText.h"
#include "lost/font/FontManager.h"
#include "lost/font/BreakMode.h"
#include "lost/font/Render.h"
#include "lost/font/TextBuffer.h"

using namespace luabind;
using namespace lost;
using namespace lost::font;
using namespace lost::mesh;
using namespace lost::font::freetype;

namespace lost
{
  namespace lua
  {
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
            .def_readwrite("numLines", &RenderedText::numLines)            
            .scope
            [
              def("create", &RenderedText::create)
            ]
        ]
      ];
    }

    void LostFontFont(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<Font>("Font")
          .def_readonly("ascender", &Font::ascender)
          .def_readonly("descender", &Font::descender)
          .def_readonly("lineHeight", &Font::lineHeight)
        ]
      ];
    }


    void LostFontTrueTypeFont(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<TrueTypeFont, Font>("TrueTypeFont")
        ]
      ];
    }
    
    void LostFontManager(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<FontManager>("FontManager")
          .def("addEntry",&FontManager::addEntry)
          .def("getFont",&FontManager::getFont)
        ]
      ];
    }
    
    void LostFontBreakMode(lua_State* state)
    {
      globals(state)["lost"]["font"]["BREAKMODE_NONE"] = lost::font::BREAKMODE_NONE;
      globals(state)["lost"]["font"]["BREAKMODE_CHAR"] = lost::font::BREAKMODE_CHAR;
      globals(state)["lost"]["font"]["BREAKMODE_WORD"] = lost::font::BREAKMODE_WORD;
    }

    void LostFontRender(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          def("render", (RenderedTextPtr(*)(const std::string & inText, const FontPtr& font))&lost::font::render),
          def("render", (void(*)(const std::string & inText, const FontPtr& font, const RenderedTextPtr& target))&lost::font::render)
        ]
      ];
    }

    void LostFontTextBuffer(lua_State* state)
    {
      module(state, "lost")
      [
        namespace_("font")
        [
          class_<TextBuffer>("TextBuffer")
          .def(constructor<>())
          .def("text", &TextBuffer::text)
          .def("font", &TextBuffer::font)
          .def("renderPhysicalLine", &TextBuffer::renderPhysicalLine)
        ]
      ];
    }

    
    void LostFont(lua_State* state)
    {
      LostFontRenderedText(state);
      LostFontFont(state);
      LostFontTrueTypeFont(state);
      LostFontManager(state);
      LostFontBreakMode(state);
      LostFontRender(state);
      LostFontTextBuffer(state);
    }

  }
}

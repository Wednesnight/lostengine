#include "lost/lua/bindings/LostFont.h"
#include "lost/lua/lua.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/font/RenderedText.h"
#include "lost/font/FontManager.h"
#include "lost/font/BreakMode.h"
#include "lost/font/Render.h"
#include "lost/font/TextBuffer.h"

#include <slub/slub.h>

using namespace lost;
using namespace lost::font;
using namespace lost::mesh;
using namespace lost::font::freetype;

namespace slub {

  template<>
  struct converter<BreakMode> {
    
    static bool check(lua_State* L, int index) {
      return lua_isnumber(L, index);
    }
    
    static BreakMode get(lua_State* L, int index) {
      return (BreakMode) luaL_checkint(L, index);
    }
    
    static int push(lua_State* L, BreakMode value) {
      lua_pushinteger(L, (int) value);
      return 1;
    }
    
  };
  
}

namespace lost
{
  namespace lua
  {
  
    RenderedTextPtr clone_cast(RenderedText* p)
    {
      MeshPtr m = p->clone();
      return static_pointer_cast<RenderedText>(m);
    }
  
    void LostFontRenderedText(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .clazz<RenderedText>("RenderedText")
          .extends<Mesh>()
          .field("min", &RenderedText::min)
          .field("max", &RenderedText::max)
          .field("size", &RenderedText::size)
          .field("numLines", &RenderedText::numLines)      
          .method("clone", clone_cast)    
          .method("characterRect", &RenderedText::characterRect)  
          .function("create", &RenderedText::create);
    }

    void LostFontFont(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .clazz<Font>("Font")
          .field("ascender", &Font::ascender)
          .field("descender", &Font::descender)
          .field("lineHeight", &Font::lineHeight)
          .field("atlas", &Font::atlas)
          .field("atlasSize", &Font::atlasSize);
    }


    void LostFontTrueTypeFont(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .clazz<TrueTypeFont>("TrueTypeFont")
          .extends<Font>();
    }
    
    void LostFontManager(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .clazz<FontManager>("FontManager")
          .method("addEntry",&FontManager::addEntry)
          .method("getFont",&FontManager::getFont);
    }
    
    void LostFontBreakMode(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .enumerated("BREAKMODE_NONE", lost::font::BREAKMODE_NONE)
        .enumerated("BREAKMODE_CHAR", lost::font::BREAKMODE_CHAR)
        .enumerated("BREAKMODE_WORD", lost::font::BREAKMODE_WORD);
    }

    void LostFontRender(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .function("render", (RenderedTextPtr(*)(const string & inText, const FontPtr& font,bool characterMetrics, int align))&lost::font::render)
        .function("render", (void(*)(const string & inText, const FontPtr& font, const RenderedTextPtr& target,bool characterMetrics, int align))&lost::font::render);
    }

    void LostFontTextBuffer(lua_State* state)
    {
      slub::package(state, "lost").package("font")
        .clazz<TextBuffer>("TextBuffer")
          .constructor()

          .method("text", &TextBuffer::text)
          .method("font", &TextBuffer::font)
          .method("breakMode", &TextBuffer::breakMode)
          .method("width", &TextBuffer::width)
          .method("reset", &TextBuffer::reset)

          .method("renderPhysicalLine", &TextBuffer::renderPhysicalLine)
          .method("renderAllPhysicalLines", &TextBuffer::renderAllPhysicalLines)
          .method("numCharsInPhysicalLine", &TextBuffer::numCharsInPhysicalLine)
          .method("numLogicalLines", &TextBuffer::numLogicalLines)
          .method("numPhysicalLines", &TextBuffer::numPhysicalLines)
          .field("characterMetrics",&TextBuffer::getCharacterMetrics, &TextBuffer::setCharacterMetrics)
          .field("align",&TextBuffer::getAlign, &TextBuffer::setAlign)
          .method("insertUtf8StringAtPosition", &TextBuffer::insertUtf8StringAtPosition)
          .method("eraseCharAtPosition", &TextBuffer::eraseCharAtPosition)
          .method("eraseChars", &TextBuffer::eraseChars)
          .method("utf8String", &TextBuffer::utf8String)
          .method("substring", &TextBuffer::substring);
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

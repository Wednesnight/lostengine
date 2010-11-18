#ifndef LOST_FONT_FORWARD_H
#define LOST_FONT_FORWARD_H

#include "lost/font/textforward.h"

namespace lost
{
namespace font
{

struct FontManager;
typedef lost::shared_ptr<FontManager> FontManagerPtr;

struct TrueTypeFont;
typedef lost::shared_ptr<TrueTypeFont> TrueTypeFontPtr;

struct Font;
typedef lost::shared_ptr<Font> FontPtr;

struct Glyph;
typedef lost::shared_ptr<Glyph> GlyphPtr;

struct BitmapFont;
typedef lost::shared_ptr<BitmapFont> BitmapFontPtr;

struct RenderedText;
typedef lost::shared_ptr<RenderedText> RenderedTextPtr;

}
}
#endif
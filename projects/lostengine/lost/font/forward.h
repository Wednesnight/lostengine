#ifndef LOST_FONT_FORWARD_H
#define LOST_FONT_FORWARD_H

namespace fhtagn {
namespace text {

typedef char            utf8_char_t;
typedef std::basic_string<utf8_char_t>  utf8_string;
typedef uint16_t utf16_char_t;
typedef std::basic_string<utf16_char_t> utf16_string;
typedef uint32_t utf32_char_t;
typedef std::basic_string<utf32_char_t> utf32_string;

}
}

namespace lost
{
namespace font
{

struct TrueTypeFont;
typedef lost::shared_ptr<TrueTypeFont> TrueTypeFontPtr;

struct RenderedText;
typedef lost::shared_ptr<RenderedText> RenderedTextPtr;

}
}
#endif